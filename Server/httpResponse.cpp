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
void    GETMethod() {
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

            // open file


    // handle cgi
}

//https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/POST
void    POSTMethod() {
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
void    DELETEMethod() {
    std::cout << "\tDELETE method:\n";
    // handle path
    // remove path data
    // create response  or error handling if remove didn't work
}

void        httpResponse::methodHandler(std::string method) {
    if (method[0] == 'G')
        GETMethod();
    else if (method[0] == 'P')
        POSTMethod();
    else if (method[0] == 'D')
        DELETEMethod();
    else
        std::cout << "not get not post not delete." << std::endl;
}



void   httpResponse::setContentType(std::string type) {
    _contentType = type;
}

std::string httpResponse::getContentType() {
    return _contentType;
}