#include "testServer.hpp"

HDE::testServer::testServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10) { launch(); }

void    HDE::testServer::accepter() {

    struct sockaddr_in address = getSocket()->getaddress();
    int addrlen = sizeof(address);
    newSocket = accept(getSocket()->getsock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    read(newSocket, buffer, 30000);
}

void    HDE::testServer::handler() {

    std::cout << buffer << std::endl;
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