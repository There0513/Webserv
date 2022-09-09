#include "httpResponse.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

httpResponse::httpResponse() {
}

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
    if (_contentType == "html")
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
    // check for cgi        ◦ Execute CGI based on certain file extension (for example .php).
    // if !cgi:

        // find resource    handle path (location, root, ...)
        // check if file exists with stat()
        // if path == directory
            // dirStream = opendir()    // handle error if !opendir()
            // find index.html via readdir(dirStream) + append path + filename
                // if !index.html -> closedir(dirStream) -> check if autoindex + generate repsponse
            // closedir(dirStream);
        // else open file + set content:
            setPageContent(request.readFileContent());
            findContentType(request.getUrl());


    // handle cgi
}

//https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/POST
void    httpResponse::POSTMethod() {
    std::cout << "\tPOSTmethod:\n";
    // check for cgi        ◦ Execute CGI based on certain file extension (for example .php).
    // if !cgi:
        // check for upload file
            // if !
                // error -> post req not accepted
            // if upload file:
                // check content-type form
                    // 'form-data' upload:
                        // extract boundary string from Content-Type
                        // ex: Content-Type: multipart/form-data;boundary="boundary"
                        // handle upload - create file etc

        // create a file with raw data from body


    // handle cgi
}

// The DELETE method deletes the specified resource.
void    httpResponse::DELETEMethod() {
    std::cout << "\tDELETE method:\n";
    // handle path
    // remove path data
    // create response  or error handling if remove didn't work
}

void        httpResponse::methodHandler(std::string method) {
    // get location from config
    if (checkCgi() == 1)
        handleCgi();
    else if (method[0] == 'G')
        GETMethod();
    else if (method[0] == 'P')
        POSTMethod();
    else if (method[0] == 'D')
        DELETEMethod();
    else
        std::cout << "not get not post not delete." << std::endl;
}


    /* CGI */

int     httpResponse::checkCgi() {
    std::cout << "\t\t\tcheckCgi()\n";
    size_t  ext;
    // std::cout << "\n\n\t\t\t~~~~~request.getUrl(): " << request.getUrl() << std::endl;

    if ((ext = request.getUrl().find_last_of(".")) != std::string::npos && request.getMethod() != "DELETE"
        // && cf.getValue(request.getHost(), request.getUrl(), cgi) != ""
        ) {
        // std::cout << "\n\n\t\t\t~~~~~ext: " << ext << std::endl;
        // check extension:
        std::string extension = request.getUrl().substr(ext + 1, request.getUrl().size());
        std::cout << "\n\n\t\t\t~~~~~extension: " << extension << std::endl;
        // check if extension is valid in configfile
        if (extension == "py" || extension == "pl")
            return 1;
    }
    return 0;
}

void    httpResponse::handleCgi() {// GO ON HERE
    std::cout << "\t\t\thandleCgi()\n";
    pid_t   pid;
    int     status;
    // std::string execStr = "www/cgi-bin/"; // + scriptname
    char* execStr = "www/cgi-bin/perl.pl"; // + scriptname
    // get cgi location from config file    execStr = cgiLocation/script.pl/py...
    // check if executable exists

    // exec with fork:
    pid = fork();
    if (pid == 0) {
        // create env variable
        // change stdin/stdout with dup2
        // https://www.ibm.com/docs/en/netcoolomnibus/8.1?topic=scripts-environment-variables-in-cgi-script
        // execve(execStr, NULL, env)
        std::cout << "\t\t\t\texecve\n";
        execve(execStr, NULL, environ);
    }
    else if (pid < 0) {
        request.setStatusCode(500);
    }
    waitpid(pid, &status, 0);
    
    // read 'tmp file'
    // put file into body
}


    /* GETTER SETTER */

void   httpResponse::setContentType(std::string type) {
    _contentType = type;
}

std::string httpResponse::getContentType() {
    return _contentType;
}