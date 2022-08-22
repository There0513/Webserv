#include "testServer.hpp"
#include "httpRequest.hpp"
#include "httpResponse.hpp"
#include "httpResponse.cpp"
#include "Client.hpp"
#include "Client.cpp"
#include <fcntl.h>
/*  notes:
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
*/

/*!!!!!!!!!!!!!!!!!!!!!!!!    MAIN LOOP :  !!!!!!!!!!!!!!!!!!!!!!!!

set server
while
{
accept new connections for each server...

    while server.host != server.host.end {          // check events for each server
        while clients                              // check events for each client
        {
            // handle events( , , )
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
    }
}
close fds + destroy servers

*/
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