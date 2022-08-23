#include "testServer.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include "httpResponse.cpp"
#include "Client.hpp"
#include "Client.cpp"
#include <fcntl.h>

HDE::testServer::testServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10) {
    launch();
}

void    HDE::testServer::accepter() {
// while servers: -> accept + save newSocket in each client-class for later
    // Client *newClient = new Client();
    struct sockaddr_in address = getSocket()->getaddress();
    int addrlen = sizeof(address);
    newSocket = accept(getSocket()->getsock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    // if (newSocket != -1) {
        // newClient->setSocket(newSocket);
    //     fcntl(newSocket, F_SETFL, O_NONBLOCK); // subject: "However, you are allowed to use fcntl() only as follows: fcntl(fd, F_SETFL, O_NONBLOCK);"
        FD_SET(newSocket, &FD_read); // put _fd_set in server class     https://linux.die.net/man/3/fd_set
        // clients[newSocket] = newClient;
    // }
}

// void    HDE::testServer::handler() {    // => methodHandler() in response
    // httpResponse        response;
    // httpRequest request(buffer, newSocket);    // parse request-string into 'httpRequest request'
    // parsing done - handle methods here if get else if ....
    // ;
    // if (request.getMethod()[0] == 'G')
    //     GETMethod();
    // else if (request.getMethod()[0] == 'P')
    //     POSTMethod();
    // else if (request.getMethod()[0] == 'D')
    //     DELETEMethod();
    // else
    //     std::cout << "not get not post not delete." << std::endl;
    // before methods or in method-function?
    // response.setPageContent(request.readFileContent());
    // request.findContentType();
    // prepare response:
    // responder(response.getPageContent(), request.getContentType());
// }

void    HDE::testServer::responder(std::string content, std::string contentType) {
    // process the request - create response
    // handle method (GET POST DELETE)
    // std::cout << "page-content = |" << content << "|" << std::endl;
    std::string answer = "HTTP/1.1 200 OK\nContent-Type: ";
    // add code + code description
    answer+= contentType;
    answer+= "; charset=UTF-8\nContent-Length:";
    answer+= std::to_string(content.length());
    answer+= "\n\n";
    answer+= content;


    //  WRITE
    // send response
    // std::cout << "answer = [" << answer << "]\n" << std::endl;
    write(newSocket, answer.c_str(), answer.size());
    //  CLOSE
    close(newSocket);
}
int     HDE::testServer::readingTmp() {
    _ret = read(newSocket, buffer, 30000); // read or recv
    if (_ret <= 0) {
        close(newSocket);
        // FD_CLR(_socket, &fd_set); //      FD_CLR() removes a given file descriptor from a set (https://linux.die.net/man/3/fd_clr) fd_set is a fixed size buffer
        // handle error (INTERNAL_SERVER_ERROR = 500)
            // generate response with error page with or without auto
        // erase _socket from sockets-map and restart loop through socket-map
        return -1;
    }
    buffer[_ret] = '\0';
    
    return 1;
}
void    HDE::testServer::launch() {

    httpResponse        response;
    while (true)
    {
        std::cout << "====== Waiting for the connection =====" << std::endl;
        // select
        accepter(); // accept new connections for each server + save result-socket in each client-class
        // while clients: {
            // if client ready to READ -> if FD_ISSET(newSocket, readingSet)     // ft_set readingSet -> arg in select(.., &readingSet, &writingSet, .., ..)
            if (FD_ISSET(newSocket, &FD_read))
                if (readingTmp() != -1) {          //     read/recv      clients[newSocket] tmp -> dynamically in while
                // if (clients[newSocket]->reading() != -1) {          //     read/recv      clients[newSocket] tmp -> dynamically in while
                    httpRequest request(buffer, newSocket);    // parse request-string into 'httpRequest request'
                    // httpRequest request(*(clients[newSocket])->_buffer, newSocket);    // parse request-string into 'httpRequest request'
                    if (request.isValid() != -1) {// check if valid
                        // apply - location/root (here or in getfirstline()?) - alias - queries 
                        response.setPageContent(request.readFileContent());
                        response.findContentType(request.getUrl());
                        // recheck valid status code
                        response.methodHandler(request.getMethod());
                        responder(response.getPageContent(), response.getContentType()); // later in respond() 

                    }
                }
            // }
            // if client ready to write {  FD_ISSET(..) with fd-for-write
            //      respond
                    // responder(response.getPageContent(), request.getContentType());

            // }
            // else {
            //     ... do not time out
            // }
        // }
        std::cout << "===== Done =====" << std::endl;
        // break ;
    }
    
}