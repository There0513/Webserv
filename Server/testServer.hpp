#ifndef testServer_HPP
# define testServer_HPP

#include <stdio.h>
#include "SimpleServer.hpp"

namespace HDE {

    class testServer : public SimpleServer {

        private:
            char    buffer[30000] = {0};
            int     newSocket;
            void    accepter();
            void    handler();
            void    responder();

        public:
            testServer();
            void    launch();
    };
}

#endif