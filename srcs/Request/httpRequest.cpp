#include "httpRequest.hpp"
#include <fstream>
#include <sstream>      // ostringstream
#include <dirent.h>     // DIR
#include <sys/stat.h>   // stat
#include "../Config/colormod.hpp"

Color::Modifier		rouge(Color::FG_RED);
Color::Modifier		defi(Color::FG_DEFAULT);

httpRequest::httpRequest(std::string buffer, long socket): _method(""), _url(""), _version(""), _body(""),
_statusCode(0), _isChunked(false), _auto(false) {
    parseRequest(buffer);
}

httpRequest::httpRequest(void) {}

httpRequest::~httpRequest() {}

std::string         httpRequest::readDirectoryAutoindex() {
    const char  *path = _url.c_str();
    std::string dirName(path);
    DIR         *dir = opendir(path);

    if (dir == NULL) {
        std::cerr << "Error: could not open [" << path << "]" << std::endl;
        return "";
    }
    
    std::string root;
    
    try {
        root = _ConfigFile->getValue("localhost:8080", "/", "root")[0];
    }
    catch (ConfigFile::ValueNotFoundException &e) {
        root = "";
    }
    if (root == "") {
        try {
            root = _ConfigFile->getValue("localhost:8080", "", "root")[0];
        }
        catch (ConfigFile::ValueNotFoundException &e) {
           root = "";
        }
    }
    if (dirName[0] != '/')
        dirName = "/" + dirName;
    std::cout << "\tdirName: " << dirName << "\n\troot: " << root << "\n";
    if (dirName == ("/"+root))
        dirName = "";
    size_t  pos;
    if ((pos = dirName.find("/"+root)) != std::string::npos) {
        dirName = dirName.substr(root.size() + 1, dirName.size());
    }
    std::cout << "\tdirName: " << dirName << "\n";

    std::string page ="<!DOCTYPE html>\n<html>\n<head>\n<title>" + dirName + "</title>\n</head>\n<body>\n<ul><h1>" + dirName + "</h1";
    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
        page += "\t\t<li><a style=\"text-decoration: none;color:black;\" href=" + dirName + "/" + std::string(dirEntry->d_name)\
                + "\><strong>" + std::string(dirEntry->d_name) + "</strong></a></li>\n";
    page +="</ul>\n</body>\n</html>\n";
    closedir(dir);
    _auto = true;
    return page;
}

std::string     httpRequest::readFileContent() {
    std::ifstream       data;
    std::ostringstream  buffer;

    data.open(_url);
    if (!data) {
        std::cout << "Error: " << _url << " could not be opened. Send tmp error page." << std::endl;
        _url = "srcs/Server/www/errorPages/404notfound.html";
        data.open(_url);
    }
    buffer << data.rdbuf();  // reading data
    data.close();
    return buffer.str();
}

//https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
std::string     httpRequest::readContent() {
    struct stat         s;

    std::cout << "\t\t\t_url beginn of readContent(): " << _url << std::endl;
    if (_url[0] == '/' && _url.length() > 1)
        _url = _url.substr(1, _url.length());
    if (_url[0] == '/' && _url.length() == 1)
        _url = "index.html";

    if (stat(_url.c_str(), &s) == 0) //Get file attributes for FILE and put them in 's'.
    {
        if (s.st_mode & S_IFDIR) {
            std::cerr << "it's a directory\n";
            return readDirectoryAutoindex();
        }
        else if (s.st_mode & S_IFREG) {
            std::cerr << "it's a file\n";
            return readFileContent();
        }
        else
            std::cerr << "something else\n";
    }
    else
            std::cerr << "\t|favicon|error: else of if(stat(_url.c_str(),&s) == 0 )\n";
    std::cout << "|favicon|end of readContent\n";
    return "";
}

int     httpRequest::checkFirstLine() {
// check method:
    if (_method.compare("GET") != 0 && _method.compare("POST") != 0 && _method.compare("DELETE") != 0)
    {
        std::cerr << "Error: method not valid." << std::endl;
        // change status code + return -1
        return -1;
    }
// check url:
    std::cout << "_url[0] = " << _url[0] << "|\n";
    if (_url[0] != '/' || _url.length() == 0) {
        _statusCode = 400; // bad request
        return -1;
    }
    if (_url.length() >= 256) {
        _statusCode = 414;
        return -1;
    }

// check http version:
    if (_version.compare(0, 6, "HTTP/1") != 0) {
        std::cerr << "Error: HTTP Version not valid." << std::endl;
        _statusCode = 505;  // "HTTP Version not supported"
        return -1;
    }
    // check if numbers after 'HTTP/1':
    if (_version.substr(7, _version.length()).find_first_not_of("0123456789") != std::string::npos) {
        std::cerr << "Error: HTTP Version not valid." << std::endl;
        _statusCode = 505;  // "HTTP Version not supported"
        return -1;
    }
    std::cout << "\t\t\t\tcheckFirstLine OK\n";
    return 1;
}

void    httpRequest::getFirstLine(std::string str, std::string deli = " ")
{
    int start = 0;
    int end = str.find(deli);

    if (end != -1)
        _method = str.substr(start, end - start);
    start = end + deli.size();
    end = str.find(deli, start);
    if (end != -1)
        _url = str.substr(start, end - start);
    start = end + deli.size();
    end = str.find("\r\n", start);
    if (end != -1)
        _version = str.substr(start, end - start);
    std::cout << "_method: |" << _method << "|" << std::endl;
    std::cout << "_url: |" << _url << "|" << std::endl;
    std::cout << "_version: |" << _version << "|" << std::endl;
}

void    httpRequest::parseHeader(std::string buffer) {
    size_t      points;
    std::string key;
    std::string value;
    
    if (buffer.find("\r\n") == 0)       // erase first empty line
                buffer.erase(0, 1);
    std::string    line = buffer.substr(0, buffer.find("\r\n"));

    while (line != "")
    {
        points = line.find_first_of(":");
        key = line.substr(1, points);   // without \n
        value = line.substr(points + 1, line.length());
        if (key != "" && value != "")
            _header.push_back(std::make_pair(key, value));
        buffer.erase(0, line.length() + 1);     // delete first line from buffer
        line = buffer.substr(0, buffer.find("\r\n"));
    }
    // print _header:
    // for (std::vector<std::pair<std::string, std::string> >::const_iterator it = _header.begin(); it != _header.end(); ++it)
    //     std::cout << "key: " << it->first << " : val: " << it->second << "\n";
}

void    httpRequest::parseBody() {  // already set in parseRequest
    std::cout << "_body: |" << _body << "|" << std::endl;
    // if 'Content-Length' check if parseBody needed +++
}

int     httpRequest::isValid(ConfigFile & cf) {
    _ConfigFile = &cf;
    // if POST: content-length header
    try {        
        if (checkFirstLine() == -1) { // if -1  -> return/send error response
            std::cout << "ERROR: Invalid request" << std::endl;
            _url = cf.getErrorPage(_host, "400");
        }
        if (cf.isMethodAllowed(_host, _url, _method) == false) { // check if method is allowed
            std::cout << rouge << "ERROR: Method is not allowed for this server" << defi << std::endl;
            _url = cf.getErrorPage(_host, "400");
        }
        // Check if there is a cgi, and if yes, if the config file is set up for it
        size_t ext = _url.find_last_of(".");
        if (ext == std::string::npos)
            return 1;
        _extension = _url.substr(ext + 1, _url.size());
        if (_extension == "py" || _extension == "pl" || _extension == "php") {
            cf.getValue(_host, _url, "cgi");
            isCgi = true;
        }
    }
    catch (ConfigFile::ServerNotFoundException &e) {
        std::cout << rouge << e.what() << defi << std::endl;
        _url = cf.getErrorPage(_host, "404");
    }
    catch (ConfigFile::ValueNotFoundException &e) {
        std::cout << rouge << e.what() << ". Check the URI of your request." << defi << std::endl;
        _url = cf.getErrorPage(_host, "404");
    }
    // min/max length content --> only for POST method (?)
    return 1;   // all good
}

void    httpRequest::setQuery() {
    size_t  pos;

    if ((pos = _url.find_first_of('?')) != std::string::npos){
        _query = _url.substr(pos + 1, _url.size());
        _url = _url.substr(0, pos);
    }
    std::cout << "query: |" << _query << "|\n";
    std::cout << "new _url: |" << _url << "|\n";
}

/*  chunked:
Data is sent in a series of chunks. The Content-Length header is omitted in this case and at the beginning of each chunk you
need to add the length of the current chunk in hexadecimal format, followed by '\r\n' and then the chunk itself, followed by
another '\r\n'. The terminating chunk is a regular chunk, with the exception that its length is zero. It is followed by the
trailer, which consists of a (possibly empty) sequence of header fields.
https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Transfer-Encoding#chunked_encoding
https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Transfer-Encoding
https://stackoverflow.com/questions/24625620/how-should-http-server-respond-to-head-request-for-chunked-encoding
*/
void    httpRequest::parseRequest(std::string buffer) {
    int start = buffer.find("\r\n\r\n");
    int end = buffer.size();

    if (start != std::string::npos) {    // check if "/r/n/r/n" present
        _body = buffer.substr(start + 4, end -1);

        getFirstLine(buffer);
        setQuery();
        start = buffer.find("\r\n");
        if (start != std::string::npos)
            buffer = buffer.substr(start, end);
        // std::cout << "new buffer for parseHeader = |" << buffer << "|" << std::endl;
        parseHeader(buffer);
        // get server informations for port/host from conf file?
        std::string *val = getHeaderValue("Transfer-Encoding");
        if (val) {
            if (val->find("chunked") != std::string::npos)
                _isChunked = true;  // -> first line is hexadecimal value that tells the extraction length of the second line
        }
        // else if (getHeaderValue("Content-Length")) {
        // }
        /*
        The Content-Length is optional in an HTTP request. For a GET or DELETE the length must be zero.
        For POST, if Content-Length is specified and it does not match the length of the message-line,
        the message is either truncated, or padded with nulls to the specified length.
        The Content-Length is always returned in the HTTP response even when there is no content, in which case the value is zero.
        */
        setHost(buffer);
        parseBody();
    }
}

void    httpRequest::handleURL(ConfigFile & cf) {   // find url-corresponding route

    try {

        if (_url.find("error") == std::string::npos)
            _url = cf.findPath(_host, _url); // find the path to the right file inside the server

    }
    catch (ConfigFile::ServerNotFoundException &e) {
        std::cout << rouge << e.what() << defi << std::endl;
    }
    catch (ConfigFile::ValueNotFoundException &e) {
        std::cout << rouge << e.what() << defi << std::endl;
    }
    // check if query '? =' in _url
    // if root in config.file -> add root:
        // std::string tmp_root = "www";                // getRoot() from config.file ==> CAPU ==> getValue(host(string), url(string), "root") See line 64 of main.cpp (use a try and catch structure)
        // _url = tmp_root + "/" + _url;

    // apply - location/root - alias - queries 
}

/* SETTERS - GETTERS */

 void       httpRequest::setUrl(std::string url) {
    _url = url;
 }

std::string httpRequest::getUrl() {
    return _url;
}

void    httpRequest::setHost(std::string buffer) {
    // get the Host of the request
    int start = buffer.find_first_of(":") + 2;
    
    if ((start != std::string::npos)) {
        std::string tmp = buffer.substr(start, buffer.length());
        _host = buffer.substr(start, tmp.find('\n'));
    }
}

std::string    httpRequest::getHost() {
    return _host;
}


std::string httpRequest::getBody() {
    return _body;
}

std::string httpRequest::getExtension() {
    return _extension;
}

void    httpRequest::setMethod(std::string method) {
    _method = method;
}

std::string httpRequest::getMethod() {
    return _method;
}

void    httpRequest::setStatusCode(int StatusCode) {
    _statusCode = StatusCode;
}

int     httpRequest::getStatusCode() {
    return _statusCode;
}

std::string *httpRequest::getHeaderValue(std::string const &key) {
	for (std::vector<std::pair<std::string, std::string> >::iterator
			 it = _header.begin();
		 it != _header.end(); ++it)
	{
		if (it->first == key)
			return (&it->second);
	}
	return (NULL);
}

void        httpRequest::setHeaderValue(std::string key, std::string value) {
    _header.push_back(std::make_pair(key, value));
}