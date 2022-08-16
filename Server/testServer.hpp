#ifndef testServer_HPP
# define testServer_HPP

#include <stdio.h>
#include "SimpleServer.hpp"
#include "httpRequest.hpp"


namespace HDE {

    class testServer : public SimpleServer {

        private:
            char    buffer[30000] = {0};
            int     newSocket;
            void    accepter();
            void    handler();
            void    responder(std::string content, std::string contentType);

            httpRequest     requests();

        public:
            testServer();
            void    launch();
    };
}

#endif