#ifndef testServer_HPP
# define testServer_HPP

#include <stdio.h>
#include "SimpleServer.hpp"
#include "Client.hpp"
#include "httpRequest.hpp"


namespace HDE {

    class testServer : public SimpleServer {

        private:
            char    buffer[30000] = {0};
            int     newSocket;
            long    _ret;    // return of read()
            fd_set  FD_read;
            fd_set  FD_write;
            void    accepter();
            // void    handler();       // => methodHandler() in response
            void    responder(std::string content, std::string contentType);

            std::map<int, Client*>  clients;

            httpRequest             requests();


        public:
            testServer();
            void    launch();

            // tmp:
            int     readingTmp();
    };
}

#endif