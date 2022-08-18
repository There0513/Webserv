#include "httpRequest.hpp"
#include <fstream>
#include <sstream> // ostringstream

httpRequest::httpRequest(std::string buffer, long socket) {
    std::cout << "\nhttpRequest [ " << buffer << " ] END httpRequest\n" << std::endl;
	// requests.insert(std::make_pair(socket, "")); // init requests          ~ earlier in loop maybe?
    // requests[socket] += std::string(buffer);    // requests map: <socket, bufferstring>
    // parse buffer (method, url, version, headerFields, body) -> add header:
    parseRequest(buffer);
    // find url-corresponding route
    // check if request is valid (http version, host, method(valid one + allowed in config.file + if POST: content-length header))
    // redirection?
}

httpRequest::httpRequest(void) {}
httpRequest::~httpRequest() {}

//https://www.tutorialspoint.com/Read-whole-ASCII-file-into-Cplusplus-std-string
std::string     httpRequest::readFileContent() {
    std::ifstream       data;
    std::ostringstream  buffer;

    data.open(_url);
    if (!data) {
        std::cout << "Error: " << _url << " could not be opened. Send tmp error page." << std::endl;
        _url = "www/errorPages/404notfound.html";
        data.open(_url);
    }
    buffer << data.rdbuf();  // reading data
    data.close();
    return buffer.str();
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
        _url = str.substr(start+1, end - start - 1);
    start = end + deli.size();
    end = str.find(deli, start);
    if (end != -1)
        _version = str.substr(start, end - start).substr(5, 4);
    if (_url.size() <= 1)
            _url = "index.html";
    // if root in config.file -> add root:
    std::string tmp_root = "www";  // getRoot() from config.file
    _url = tmp_root + "/" + _url;

    std::cout << "_method: " << _method << std::endl;
    std::cout << "_url: " << _url << std::endl;
    std::cout << "_version: " << _version << std::endl;
    // while (end != -1) {
    //     std::cout << "\nsplit: " << str.substr(start, end - start) << std::endl;
    //     start = end + deli.size();
    //     end = str.find(deli, start);
    // }
    // std::cout << "\nsplit: " << str.substr(start, end - start);
}

void    httpRequest::parseHeader(std::string buffer) {
    size_t  pos;

    /*
    pos = get
    while pos != std::string::npos  // while /n/r or /n/r is not last elem
        line = getLine() + delete in buffer actual line;
            if line == empty -> end of header/begin of body
        else
            split line with delimiter = ":" -> get key + val from line // ex:  Host localhost:8080
            if key == host -> setHost = val
            push into header vector from httpRequest

    */
}

void    httpRequest::parseBody() {
    // https://stackoverflow.com/questions/24625620/how-should-http-server-respond-to-head-request-for-chunked-encoding
}

void    httpRequest::findContentType() {
	_contentType = _url.substr(_url.rfind(".") + 1, _url.size() - _url.rfind("."));
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

void    httpRequest::parseRequest(std::string buffer) {
    getFirstLine(buffer);
    parseHeader(buffer);
    parseBody();
}


/* SETTERS - GETTERS */


void   httpRequest::setContentType(std::string type) {
    _contentType = type;
}

std::string httpRequest::getContentType() {
    return _contentType;
}