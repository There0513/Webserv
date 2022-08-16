#include "testServer.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include "httpResponse.cpp"
#include <fcntl.h>
/*
needed structure to handle request part:
    - from configuration file:
        - host + port
        - root directory
        - locations             map<string, locationClass(with all informations inside? index,methods etc)>
        - default error pages   map<int, string>
        - max client body size
        - 301 redirection
        - upload option
        - autoindex option
    - map with all servers ('server_map') map<fd, server> to iterate through for (read/write to process the servers requests)
    - loop through server_map to connect (accept etc.) + save sockets of each (in map of sockets?!)
    - each server inside server_map has it's own requests, addr, fd's, listen variables

    - loop through sockets (sockets_map)
        - read into buffer
        - create httpRequest (parsing)
        - check if valid ()
        - what kind of content we are going to send? html/js/png/... -> add Content-Type: text/html ...
        - 'create' location 'folder/index.html' with root (ROOT/folder/index.html)  | handle alias etc.
        - check for index/autoindex/create index.html file content in page_content variable as string if index not found (+ autoindex on)
        - check for query parameters (ex GET: values visible in URL: '/Register.html&user=john&login=jonny')
        - add founded file (ex: index.html) to page_content variable

    - response:
        - send response with send or write


*/
HDE::testServer::testServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10) { launch(); }

void    HDE::testServer::accepter() {

    struct sockaddr_in address = getSocket()->getaddress();
    int addrlen = sizeof(address);
    newSocket = accept(getSocket()->getsock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    // if (newSocket != -1) {
    //     fcntl(newSocket, F_SETFL, O_NONBLOCK); // subject: "However, you are allowed to use fcntl() only as follows: fcntl(fd, F_SETFL, O_NONBLOCK);"
    //     // FD_SET(newSocket, &_fd_set);
    // }
// read the clients request:
  // if ready to READ      ->  if FD_ISSET(newSocket, readingSet)     // ft_set readingSet -> arg in select(.., &readingSet, &writingSet, .., ..)
    long ret = read(newSocket, buffer, 30000); // read or recv
}

void    HDE::testServer::handler() {
    httpResponse        response;
        /* if ret <= 0
            close(newSocket)
            FD_CLR(newSocket, &fd_set);       FD_CLR() removes a given file descriptor from a set (https://linux.die.net/man/3/fd_clr) fd_set is a fixed size buffer
            erase newSocket from sockets-map and restart loop through socket-map */
        // else:
            httpRequest request(buffer, newSocket);    // parse request-string into 'httpRequest request'
    // read content of file e.g.:
    response.setPageContent(request.readFileContent());
    request.findContentType();
    // prepare response:
    responder(response.getPageContent(), request.getContentType());
}

void    HDE::testServer::responder(std::string content, std::string contentType) {
    // process the request - create response
    // handle method (GET POST DELETE)
    // std::cout << "page-content = |" << content << "|" << std::endl;
    std::string answer = "HTTP/1.1 200 OK\nContent-Type: ";
    answer+= contentType;
    answer+= "; charset=UTF-8\nContent-Length:";
    answer+= std::to_string(content.length());
    answer+= "\n\n";
    answer+= content;


    //  WRITE
    // send response
    std::cout << "answer = |" << answer << "|\nend\n" << std::endl;
    write(newSocket, answer.c_str(), answer.size());
    //  CLOSE
    close(newSocket);
}

void    HDE::testServer::launch() {

    while (true)
    {
        std::cout << "====== Waiting for the connection =====" << std::endl;
        accepter();
        handler();
        // responder(); // called in handler()
        std::cout << "===== Done =====" << std::endl;
    }
    
}