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
                        responder(response.getPageContent(), response.getContentType()); // later in respond() just here for testing

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

void    HDE::testServer::responder(std::string content, std::string contentType) {  // new handleResponse(..)
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






// new functions from capucine branch:

void    HDE::testServer::deal_with_data(int listnum) {

    if (read(connectList[listnum], buffer, 30000) < 0) {
        std::cout << "Connexion failed" << std::endl; // connection closed, close this end
        close(connectList[listnum]);
        connectList[listnum] = 0;
    }
    else {

        std::cout << buffer << std::endl;
        char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 25\n\nHello to the world around";
        write(connectList[listnum], hello, strlen(hello));
        std::cout << "Responded" << std::endl;
    }
}

void    HDE::testServer::buildSelectList() {

    int listnum; // current items in connect list for the FOR loop

/* First, put together fd_set for select(), which will consist of the sock variables in case a new connection is coming in + all the sockets we have already accepted
    FD_ZERO clears out the fd_set called socks so that it does not contain any fd */

    FD_ZERO(&socks);

/* FD_SET() adds the fd sock to the fd set so that select() will return if a connection comes in on that socket */

    FD_SET(getSocket()->getsock(), &socks);

/* Loop through all the possible connections and add those to the fd set */

    for (listnum = 0; listnum < 10; listnum++) {

        if (connectList[listnum] != 0) {

            FD_SET(connectList[listnum], &socks);
            if (connectList[listnum] > highSocket)
                highSocket = connectList[listnum];
        }
    }

}

void    HDE::testServer::setNonBlocking(int sock) {

    int opts = fcntl(sock, F_GETFL);

    if (opts < 0) {
        perror("fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(F_SETFL)");
        exit(EXIT_FAILURE);
    }
    return;
}