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

    data.open(url);
    if (!data)
        std::cout << "Error: file could not be opened." << std::endl;
    buffer << data.rdbuf();  // reading data
    data.close();
    return buffer.str();
}

void    httpRequest::splitString(std::string str, std::string deli = " ")
{
    int start = 0;
    int end = str.find(deli);
    while (end != -1) {
        std::cout << str.substr(start, end - start) << std::endl;
        start = end + deli.size();
        end = str.find(deli, start);
    }
    std::cout << str.substr(start, end - start);
}

void    httpRequest::parseRequest(std::string buffer) {
    splitString(buffer);
}