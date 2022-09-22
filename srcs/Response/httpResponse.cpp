#include "httpResponse.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>      // std::stringstream


httpResponse::httpResponse() {}

httpResponse::~httpResponse() {}

void    httpResponse::setPageContent(std::string content) {
    pageContent = content;
}

std::string     httpResponse::getPageContent() {
    return pageContent;
}

void    httpResponse::findContentType(std::string url) {
	_contentType = url.substr(url.rfind(".") + 1, url.size() - url.rfind("."));
    std::cout << "_contentType: " << _contentType << std::endl;
    if (request._auto == true || _contentType == "html")  // autoindex
		_contentType = "text/html";
	else if (_contentType == "png")
		_contentType = "image/png";
	else if (_contentType == "css")
		_contentType = "text/css";
	else if (_contentType == "jpeg" || _contentType == "jpg")
		_contentType = "image/jpeg";
	else if (_contentType == "js")
		_contentType = "text/javascript";
	else if (_contentType == "bmp")
		_contentType = "image/bmp";
	else
		_contentType = "text/plain";
    std::cout << "_contentType: " << _contentType << std::endl;
}

            /* handle methods */
void    httpResponse::GETMethod() {
    std::cout << "\tGET method:\n";
        // find resource    handle path (location, root, ...)
        // check if file exists with stat()
        // if path == directory
            // dirStream = opendir()    // handle error if !opendir()
            // find index.html via readdir(dirStream) + append path + filename
                // if !index.html -> closedir(dirStream) -> check if autoindex + generate repsponse
            // closedir(dirStream);
        // else open file + set content:
            setPageContent(request.readContent());
            findContentType(request.getUrl());
}

//https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/POST
void    httpResponse::POSTMethod(ConfigFile * cf) {
    std::cout << "\tPOSTmethod:\n";
        // check for upload file
            // if !
                // error -> post req not accepted
            // if upload file:
                // check content-type form
                    // 'form-data' upload:
                        // extract boundary string from Content-Type
                        // ex: Content-Type: multipart/form-data;boundary="boundary"
                        // handle upload - create file etc


        std::vector<std::string> upload = cf->getValue(request.getHost(), "", "upload_path");
        std::cout << "upload path = " << upload[0] << std::endl;
        // create a file with raw data from body
        // std::string pathwithnewfile("/mnt/nfs/homes/threiss/ourWebserv/srcs/Server/www/uploads/newfile.pdf");
        std::cout << getenv("PWD") << "\n";
        exit(1);
        std::string pathwithnewfile("/home/theresa/code/42/42_cursus/ourWebserv/srcs/Server/www/uploads/newfile");
        std::ofstream newFile(pathwithnewfile.c_str());
        if(newFile)
        {
            std::string test = request.getBody();
            newFile.write(test.c_str(), test.size());
        }
        setPageContent(request.readContent());
        findContentType(request.getUrl());
}

// The DELETE method deletes the specified resource.
void    httpResponse::DELETEMethod() {
    std::cout << "\tDELETE method:\n";
    // check if file:
    struct stat s;
	if (stat(request.getUrl().c_str(), &s) == 0 )
	{
		if (s.st_mode & S_IFREG) {   // file
            if (remove(request.getUrl().c_str()) != 0)
                request.setStatusCode(403);
        }
		else if (s.st_mode & S_IFDIR)    // directory
            request.setStatusCode(404);
		else
            request.setStatusCode(404);
	}
	else
            request.setStatusCode(403);
    // create response  or error handling if remove didn't work
    if (request.getStatusCode() < 400)
        request.setUrl("/srcs/Server/www/DeletOK.html");
    else
        // _url = cf.getErrorPage(_host, "404");
        // ConfigFile * cf = request.getConfigFile();
        request.setUrl(request.getConfigFile()->getErrorPage(request.getHost(), std::to_string(request.getStatusCode())));
    setPageContent(request.readContent());
    findContentType(request.getUrl());
}

void        httpResponse::methodHandler(ConfigFile * cf, std::string method) {
    if (checkCgi() == 1)
        handleCgi();
    else if (method[0] == 'G')
        GETMethod();
    else if (method[0] == 'P')
        POSTMethod(cf);
    else if (method[0] == 'D')
        DELETEMethod();
    else
        std::cout << "not get not post not delete." << std::endl;
}


    /* CGI */

int     httpResponse::checkCgi() {
    std::cout << "\t\t\tcheckCgi()\n";
    size_t  ext;

    if (request.isCgi == true && request.getMethod() != "DELETE") {     // tmp (== true) muted to test cgi (theresa)
        // check extension:
        std::cout << "\n\n\t\t\t~~~~~extension: " << request.getExtension() << std::endl;
    	execArgv = (char **)malloc(sizeof(char *) * 3);
    	*(execArgv + 2) = (char *)malloc(sizeof(char) * 1);
    	*(execArgv + 2) = NULL;
	        *(execArgv + 0) = (char *)strdup("py");

        if (request.getExtension() == "py")
	        *(execArgv + 0) = (char *)strdup("/usr/bin/python");
        else if (request.getExtension() == "pl")
	        *(execArgv + 0) = (char *)strdup("/usr/bin/perl");
        else if (request.getExtension() == "php")
	        *(execArgv + 0) = (char *)strdup("/usr/bin/php");
        else
            return 0;
        return 1;
    }
    return 0;
}

void    httpResponse::handleCgi() {
    executeCgi();
    handleCgiFile();
}


/* unistd.h
    Standard file descriptors.
#define	STDIN_FILENO	0	Standard input.
#define	STDOUT_FILENO	1	Standard output.
#define	STDERR_FILENO	2	Standard error output.
*/
// https://www.man7.org/linux/man-pages/man3/tmpfile.3.html tmpfile()?!
// execve 3 arguments: the path to the program, a pointer to a null-terminated array of argument strings, and a pointer to a null-terminated array of environment variable strings
// execve arg[0] = cgi-bin binary arg[1] = cgi-bin script executable arg[2] = NULL
int httpResponse::executeCgi() {
    pid_t   pid;
    int     status;
    int     newFdOut;
	char *emptyempty[] = { "", NULL };  // tmp find solution
    
    // char* execArgv = "./srcs/Server/www/cgi-bin/perl.pl"; // + scriptname  | tmp
    std::cout << "url in executeCgi = " << request.getUrl() << "\n";
    if (request.getUrl()[0] == '/')
        *(execArgv + 1) = (char *)strdup(request.getUrl().substr(1, request.getUrl().size()).c_str());
    else
        *(execArgv + 1) = (char *)strdup(request.getUrl().substr(0, request.getUrl().size()).c_str());

    // get cgi location from config file    execArgv = cgiLocation/script.pl/py...
    // check if executable exists

    pid = fork();
    if (!pid) {
        createEnvVar();
        close(STDIN_FILENO);        // close stdin -> new stdin == response from execve
        newFdOut = open("./cgiFile", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);// == new stdout
        if (newFdOut == -1)
            exit(1);
        dup2(newFdOut, STDOUT_FILENO);  // change stdin/stdout with dup2
     
        if ((execve(execArgv[0], execArgv, _envVar)) < 0) {
            std::cout << "execve failed.\n";
            exit(1);
        }
    }
    else if (pid < 0) {
        request.setStatusCode(500);
        return -1;
    }
    waitpid(pid, &status, 0);
    return 1;
    // return WEXITSTATUS(status); // Obtain exit status of a child process
}

// read 'tmp file' + set Page content
void    httpResponse::handleCgiFile() {
    std::fstream    file;
    std::string     line;
    std::stringstream   sStream;

    file.open("cgiFile");

    if (!file)
        std::cerr << "open cgiFile error.\n";
    else {
        if (getline(file, line))
            if (line.find("Content-type")) { // check if Content-type in first line for header
                size_t points = line.find_first_of(":");
                std::string key = line.substr(0, points);   // without \n
                std::string value = line.substr(points + 1, line.length());
                if (key != "" && value != "")
                    request.setHeaderValue(key, value);
            }
        while (getline(file, line)) // skip first line if content-type
            sStream << line + "\n";
    }
    file.close();
    setPageContent(sStream.str());
}
// https://www.ibm.com/docs/en/netcoolomnibus/8.1?topic=scripts-environment-variables-in-cgi-script
// https://darrencgi.tripod.com/env_var.html
void    httpResponse::createEnvVar() {
    std::map<std::string, std::string>  _env;
  
  // tmp values:
    if (request.getHeaderValue("content-length"))
        _env["CONTENT_LENGTH"] = request.getHeaderValue("content-length")->c_str();
    else
        _env["CONTENT_LENGTH"] = "0";
    // _env["CONTENT_TYPE"] = request.getHeaderValue("content-type")->c_str();
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["REDIRECT_STATUS"] = request.getStatusCode();
    _env["REQUEST_METHOD"] = request.getMethod().c_str();
    _env["QUERY_STRING"] = request.getBody().c_str();   // ex.: "first=Anna&last=REISS"
    _env["SCRIPT_NAME"] = "srcs/Server/www/cgi-bin/python.py";
    _env["SERVER_NAME"] = "0";
    _env["SERVER_PORT"] = request.getHost().c_str();
    _env["SERVER_PROTOCOL"] = request.getVersion().c_str();
    _env["SERVER_SOFTWARE"] = "xy/1.0";

	_envVar = new char*[_env.size() + 1];
    int	i = 0;
	for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); it++) {
		std::string	element = it->first + "=" + it->second;
		_envVar[i] = new char[element.size() + 1];
		_envVar[i] = strcpy(_envVar[i], (const char*)element.c_str());
		i++;
	}
	_envVar[i] = NULL;
}

    /* GETTER SETTER */

void   httpResponse::setContentType(std::string type) {
    _contentType = type;
}

std::string httpResponse::getContentType() {
    return _contentType;
}