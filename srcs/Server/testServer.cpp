#include "testServer.hpp"
#include "../Request/httpRequest.hpp"
#include "../Response/httpResponse.hpp"
#include "../utils/utils.cpp"
#include <fcntl.h>

HDE::testServer::testServer(ConfigFile cf) : SimpleServer(AF_INET, SOCK_STREAM, 0, cf.portsToOpen, INADDR_ANY, 10) {

    _ConfigFile = &cf;
    launch();
}

void    HDE::testServer::launch() {

    highSocket = getSocket().back()->getsock();
    memset((char *)&connectList, 0, sizeof(connectList));

    while (true)
    {
        std::cout << "====== Waiting for the connection =====" << std::endl;

        accepter();
        handler();
        responder();
        //     ... do not time out
        std::cout << "=============== Done ==================" << std::endl;
    }
}

/*  SELECT 
    1st argument: highest fd value + 1 
    2nd argument: address of the fd set that contains sockets we are waiting to be readable
    3rd argument: fd_set we want to know if you can write on
    4th argument: socket you're waiting for out-of-band data (usually not useful)
    5th argument: time out of how long select should block. Null if we want to block wait forever

    Select() returns the nb of sockets that had things going on with them
    Once it returns, the original fd_set had been modified so that it reflects the state of why select() woke up
    ex: if fd 4 was originally in the fd set and then it became readable, the fd set contains fd 4 in it */

void    HDE::testServer::accepter() {

    int readSocks;          // nb of sockets ready for reading

    buildSelectList();

    readSocks = select(highSocket + 1, &socks, (fd_set *) 0, (fd_set *) 0, NULL);

    if (readSocks < 0) {
        perror("select");
        exit(EXIT_FAILURE);
    }
    if (readSocks == 0) {
        std::cout << "." << std::endl; // nothing ready to read, just show that we're alive
        fflush(stdout);
    }
    else
        return;
}

/*  Socks() will be set with whatever socket are ready for reading.
    If a client is trying to connect to our listening socket, select() will consider that as a socket readable
    Thus if the listening socket is part of the fd_set, we have to accept a new connection*/

void    HDE::testServer::handler() {

    for (size_t i = 0; i < getSocket().size(); i++) {
        
        if (FD_ISSET(getSocket()[i]->getsock(), &socks))
            handle_new_connections(getSocket()[i]);
    }
}

void    HDE::testServer::handle_new_connections(HDE::ListeningSocket *socketToHandle) {

    struct sockaddr_in address = socketToHandle->getaddress();
    int addrlen = sizeof(address);
    int listnum;

    newSocket = accept(socketToHandle->getsock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (newSocket < 0) {

        perror("accept");
        exit(EXIT_FAILURE);
    }
    setNonBlocking(newSocket);
    for (listnum = 0; listnum < 10 && newSocket != -1; listnum++) {

        if (connectList[listnum] == 0) {
            connectList[listnum] = newSocket;
            newSocket = -1;
        }
    }
    if (newSocket == -1) {
        write(newSocket, "Sorry, this server is too busy\n", 31); // no room left in the queue
        close(newSocket);
    }
}

/* Now check connect list for available data
    Run through our sockets and check to see if anything happened with them, if so service them */

void    HDE::testServer::responder() {
    int listnum;

    for (listnum = 0; listnum < 10; listnum++) {
        if (FD_ISSET(connectList[listnum], &socks))
            deal_with_data(listnum);
        // else
            // std::cout << "nothing to deal with.\n";
    }
}

void    HDE::testServer::deal_with_data(int listnum) {
    // while ((_ret = read(connectList[listnum], buffer, 3000000)) >= 0) {
        // cp _ret bites into a buffer 
        // example: new char[3000000];
        // memcpy _ret bytes dans le tableau de char
        // push_back le tableau dans le vector

        // for (int i = 0; buffer[i]; i++) {
        //     // std::cout << buffer[i];
        //     _requestVec.push_back(buffer[i]);
        // }
    // }

    if ((_ret = read(connectList[listnum], buffer, 3000000)) < 0) {
        std::cout << "Connexion failed" << std::endl; // connection closed, close this end
        close(connectList[listnum]);
        connectList[listnum] = 0;
    }
    else if (_ret == 0) {
        std::cout << "Connexion closed" << std::endl;
        close(connectList[listnum]);
        connectList[listnum] = 0;
    }
    else {
        buffer[_ret] = '\0';
        for (int i = 0; i < _ret; i++)
            _requestVec.push_back(buffer[i]);

        // before parsing request:      need to check if end of request received rnrn
        std::string reqString(_requestVec.begin(), _requestVec.end());
        if (reqString.find("\r\n\r\n") == std::string::npos && reqString.find("----WebKitFormBoundary") == std::string::npos) {
            // std::cout << "\t\tWAIT for next line in telnet\n";
        }
        else if (reqString.find("----WebKitFormBoundary") != std::string::npos && endOfFile(reqString) != 1){       // wait till end of request during upload
            // std::cout << "\t\t\t(endOfFile(reqString) != 1) -> wait till end of request\n";
        }
        else {
            httpRequest request(reqString, connectList[listnum]);    // parse request-string into 'httpRequest request'
            if (request.isValid(*_ConfigFile) != -1) {
                std::cout << "\tbefore handleURL_url: " << request.getUrl() << "\n";
                request.handleURL(*_ConfigFile);
                std::cout << "\tafter handleURL_url: " << request.getUrl() << "\n";
                // if redirection configured
                    // set redirection status code
                    // create response (page content + content type)
                // else -> handle method-function (GET, POST, DELETE):
                _response.request = request;
                // std::cout << "test request.getUrl(): " << request.getUrl() << std::endl;
                // std::cout << "test _response.request.getUrl(): " << _response.request.getUrl() << std::endl;
                _response.methodHandler(_ConfigFile, request.getMethod());
                // std::cout << "\tafter methodHandler _url: " << request.getUrl() << "\n";
                handleResponse(_response.getPageContent(), _response.getContentType(), connectList[listnum]);
                reqString = "";
                _requestVec.clear();
            }
        }
    }
}

int     HDE::testServer::endOfFile(std::string reqString) {
    size_t first;
    size_t  sec;
    
    if ((first = reqString.find("----WebKitFormBoundary")) != std::string::npos) {              // if --- in reqstring
        if ((sec = reqString.find("----WebKitFormBoundary", first + 1)) != std::string::npos)   // seccond -- found
            if (reqString.find("----WebKitFormBoundary", sec + 1) != std::string::npos)
                if (reqString.find("----WebKitFormBoundary", sec + 1) < reqString.size())
                    return 1;
    }
    return 0;
}

void    HDE::testServer::handleResponse(std::string content, std::string contentType, int connectListSocket) {
    httpRequest req = _response.request;
    std::string answer = req.getVersion() + " ";
    answer += to_string(req.getStatusCode()) + " ";
    answer += StatusCodeInit(req.getStatusCode()) + "\n Content-Type: ";
    answer += contentType;
    answer += "; charset=UTF-8\nContent-Length:";
    answer += to_string(content.length());
    answer += "\nDate: " + getDate();
    answer += "\n\n";
    answer += content;

    std::cout << "answer = \n[" << answer << "]\n" << std::endl;
    write(connectListSocket, answer.c_str(), answer.size());
}

void    HDE::testServer::buildSelectList() {

    int listnum; // current items in connect list for the FOR loop

/* First, put together fd_set for select(), which will consist of the sock variables in case a new connection is coming in + all the sockets we have already accepted
    FD_ZERO clears out the fd_set called socks so that it does not contain any fd */

    FD_ZERO(&socks);

/* FD_SET() adds the fd sock to the fd set so that select() will return if a connection comes in on that socket */

    for (size_t i = 0; i != getSocket().size(); i++)
        FD_SET(getSocket()[i]->getsock(), &socks);

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