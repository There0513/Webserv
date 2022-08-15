#include "httpRequest.hpp"

httpRequest::httpRequest(std::string buffer, long socket) {
    std::cout << "\nhttpRequest buffer = " << buffer << "\nEND httpRequest\n" << std::endl;
	requests.insert(std::make_pair(socket, "")); // init requests
    requests[socket] += std::string(buffer);    // requests map: <socket, bufferstring>
    // parse buffer (method, url, version, headerFields, body)
    // find url-corresponding route
    // check if request is valid (http version, host, method(valid one + allowed in config.file + if POST: content-length header))
    // redirection?
}

httpRequest::~httpRequest() {}