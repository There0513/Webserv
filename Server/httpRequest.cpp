#include "httpRequest.hpp"
#include <fstream>
#include <sstream> // ostringstream

httpRequest::httpRequest(std::string buffer, long socket) {
    std::cout << "\nhttpRequest [ " << buffer << " ] END httpRequest\n" << std::endl;
	// requests.insert(std::make_pair(socket, "")); // init requests          ~ earlier in loop maybe?
    // requests[socket] += std::string(buffer);    // requests map: <socket, bufferstring>
    // parse buffer (method, url, version, headerFields, body) -> add header:

    parseRequest(buffer);
    // check if valid (here or directly in parseRequest)

    // find url-corresponding route
    // check if request is valid (http version, host, method(valid one + allowed in config.file + if POST: content-length header))
    // if redirection configured
        // set redirection status code
        // create response
    // else -> handle method-function (GET, POST, DELETE)
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
    // check if query '? =' in _url
    // if root in config.file -> add root:
    std::string tmp_root = "www";                                       // getRoot() from config.file
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
            if no 'Content-Length -> no need to parse body I think +++
        else
            split line with delimiter = ":" -> get key + val from line // ex:  Host localhost:8080
            if key == host -> setHost = val
            push into _header vector from httpRequest

    */
}

/*  chunked:
Data is sent in a series of chunks. The Content-Length header is omitted in this case and at the beginning of each chunk you
need to add the length of the current chunk in hexadecimal format, followed by '\r\n' and then the chunk itself, followed by
another '\r\n'. The terminating chunk is a regular chunk, with the exception that its length is zero. It is followed by the
trailer, which consists of a (possibly empty) sequence of header fields.
https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Transfer-Encoding#chunked_encoding */
void    httpRequest::parseBody() {
    std::cout << "_body: |" << _body << "|" << std::endl;
    // if 'Content-Length' check if parseBody needed +++
    // check if "Transfer-Encoding"
    // check 'chunked'
    // https://stackoverflow.com/questions/24625620/how-should-http-server-respond-to-head-request-for-chunked-encoding
}

int     httpRequest::isValid() {
    // method allowed
    // http version
    // min/max length content
    // etc
    return 1;   // all good
}

void    httpRequest::parseRequest(std::string buffer) {
    int start = buffer.find("\r\n\r\n");
    int end = buffer.size();

    if (start != std::string::npos) {    // check if "/r/n/r/n" present
        _body = buffer.substr(start, end -1);

        getFirstLine(buffer);
        start = buffer.find("\r\n");
        if (start != std::string::npos)
            buffer = buffer.substr(start, end);
        std::cout << "new buffer = |" << buffer << "|" << std::endl;
        parseHeader(buffer);
        parseBody();
    }
}




/* SETTERS - GETTERS */

 void       httpRequest::setUrl(std::string url) {
    _url = url;
 }

std::string httpRequest::getUrl() {
    return _url;
}

// void   httpRequest::setContentType(std::string type) {
//     _contentType = type;
// }

// std::string httpRequest::getContentType() {
//     return _contentType;
// }

void    httpRequest::setMethod(std::string method) {
    _method = method;
}

std::string httpRequest::getMethod() {
    return _method;
}