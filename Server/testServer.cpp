#include "testServer.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include "httpResponse.cpp"
#include <fcntl.h>
/*
needed structure to handle request part:
CONFIG CLASS:
    - from configuration file:
        - host + port
        - root directory
    LOCATION CLASS(?!):
        - locations             map<string, locationClass(with all informations inside? index,methods etc)>
                /   alias   locRoot index   auto    method  upload  redirection
        - default error pages   map<int, string>
        - max client body size
        - 301 redirection
        - upload option
        - autoindex option
  SERVER CLASS:
    - map with all servers ('server_map') map<fd, server> to iterate through for (read/write to process the servers requests)
    - loop through server_map to connect (accept etc.) + save sockets of each (in map of sockets?!)
    - each server inside server_map has it's own requests, addr, fd's, listen variables config

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

- POST: content-type + content-lenght needed
Le body pour GET et DELETE ça sert à rien
    on peut considérer la requête comme parsée dès qu’on a fini de mettre les headers
Pour POST on peut mettre une query pour un formulaire ou le contenu d’un texte à générer


while clients.fd
{
    if client ready to read {   FD_ISSET(..)
        read/recv
        parse
        check if valid
    }
    if client ready to write {  FD_ISSET(..)

    }
    else {
        ... do not time out
    }
}


*/
HDE::testServer::testServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10) {
    launch();
}

void    HDE::testServer::accepter() {

    struct sockaddr_in address = getSocket()->getaddress();
    int addrlen = sizeof(address);
    newSocket = accept(getSocket()->getsock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    // if (newSocket != -1) {
    //     fcntl(newSocket, F_SETFL, O_NONBLOCK); // subject: "However, you are allowed to use fcntl() only as follows: fcntl(fd, F_SETFL, O_NONBLOCK);"
    //     FD_SET(newSocket, &_fd_set); // put _fd_set in class
    // }
// read the clients request:
  // if ready to READ      ->  if FD_ISSET(newSocket, readingSet)     // ft_set readingSet -> arg in select(.., &readingSet, &writingSet, .., ..)
    _ret = read(newSocket, buffer, 30000); // read or recv
}

            /* handle methods */
void    GETMethod() {
    // check for cgi
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

void    POSTMethod() {
    // check for cgi
    // if !cgi:
        // check for upload file
            // if !
                // error -> post req not accepted
            // if upload file:
                // check content-type form
                    // 'form-data' upload:
                        // handle upload - create file etc

        // create a file with raw data from body


    // handle cgi
}

void    DELETEMethod() {
    // handle path
    // remove path data
    // create response  or error handling if remove didn't work
}

void    HDE::testServer::handler() {
    httpResponse        response;

    if (_ret <= 0) {
        close(newSocket);
        // FD_CLR(newSocket, &fd_set); //      FD_CLR() removes a given file descriptor from a set (https://linux.die.net/man/3/fd_clr) fd_set is a fixed size buffer
        // handle error (INTERNAL_SERVER_ERROR = 500)
            // generate response with error page with or without auto
        // erase newSocket from sockets-map and restart loop through socket-map
        return;
    }
    //else ret >= 1:
    httpRequest request(buffer, newSocket);    // parse request-string into 'httpRequest request'
    // parsing done - handle methods here if get else if ....
    ;

    if (request.getMethod()[0] == 'G')
        GETMethod();
    else if (request.getMethod()[0] == 'P')
        POSTMethod();
    else if (request.getMethod()[0] == 'D')
        DELETEMethod();
    else
        std::cout << "not get not post not delete." << std::endl;
    // before methods
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
    // add code + code description
    answer+= contentType;
    answer+= "; charset=UTF-8\nContent-Length:";
    answer+= std::to_string(content.length());
    answer+= "\n\n";
    answer+= content;


    //  WRITE
    // send response
    std::cout << "answer = [" << answer << "]\n" << std::endl;
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