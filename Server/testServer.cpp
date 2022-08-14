#include "testServer.hpp"
#include "httpRequest.hpp"

HDE::testServer::testServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10) { launch(); }

void    HDE::testServer::accepter() {

    struct sockaddr_in address = getSocket()->getaddress();
    int addrlen = sizeof(address);
    newSocket = accept(getSocket()->getsock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    // read the clients request:
    read(newSocket, buffer, 30000);
}

void    HDE::testServer::handler() {
    // parse request-string into httpRequest request
    httpRequest request(buffer);

    // find url-corresponding route
    // check if request is valid (http version, host, method(valid one + allowed in config.file + if POST: content-length header))
    // redirection?
    // process the request - create response
    // handle method (GET POST DELETE)
    // send response    
}

void    HDE::testServer::responder() {

    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 25\n\nHello to the world around";
    write(newSocket, hello, strlen(hello));
    close(newSocket);
}

void    HDE::testServer::launch() {

    while (true)
    {
        std::cout << "====== Waiting for the connection =====" << std::endl;
        accepter();
        handler();
        responder();
        std::cout << "===== Done =====" << std::endl;
    }
    
}