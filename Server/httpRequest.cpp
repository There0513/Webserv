#include "httpRequest.hpp"

httpRequest::httpRequest(std::string buffer) {
    std::cout << "\nhttpRequest buffer = " << buffer << "\nEND httpRequest\n" << std::endl;

    // parse buffer (method, url, version, headerFields, body)
}

httpRequest::~httpRequest() {}