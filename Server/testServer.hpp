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

    // from capucine branch:
            // void    handler();
            // void    responder();
            int    connectList[10]; // array of connected sockets
            fd_set  socks; // socket fds we want to wake up for, using select()
            int     highSocket; // highest fd


        public:
            testServer();
            void    launch();

            // tmp:
            int     readingTmp();

    // from capucine branch:
            void    buildSelectList(void);
            void    handle_new_connections();
            void    setNonBlocking(int);
            void    deal_with_data(int);
    };
}

#endif