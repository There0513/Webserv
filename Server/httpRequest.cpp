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

    std::cout << "_url.size(): " << _url.size() << "_url in readFileContent: |" << _url << "|\n";
    data.open(_url);
    if (!data)
        std::cout << "Error: " << _url << " could not be opened." << std::endl;
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

void    httpRequest::parseRequest(std::string buffer) {
    getFirstLine(buffer);
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





void   httpRequest::setContentType(std::string type) {
}

std::string httpRequest::getContentType() {
    return _contentType;
}