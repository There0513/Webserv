#include "httpResponse.hpp"

httpResponse::httpResponse() {
}

httpResponse::~httpResponse() {}

void    httpResponse::setPageContent(std::string content) {
    pageContent = content;
}

std::string     httpResponse::getPageContent() {
    return pageContent;
}