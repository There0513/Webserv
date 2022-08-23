#ifndef SimpleServer_HPP
# define SimpleServer_HPP

#include <stdio.h>
#include <unistd.h>
#include "../Sockets/SimpleSocket.hpp"
#include "../Sockets/BindingSocket.hpp"
#include "../Sockets/ListeningSocket.hpp"
#include "../Sockets/ConnectingSocket.hpp"
#include "../Sockets/SimpleSocket.cpp"
#include "../Sockets/BindingSocket.cpp"
#include "../Sockets/ListeningSocket.cpp"
#include "../Sockets/ConnectingSocket.cpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>

namespace HDE {

    class SimpleServer {

        private:
            ListeningSocket *socket;
            virtual void accepter() = 0;
            // virtual void handler() = 0;
            virtual void responder(std::string content, std::string contentType) = 0;

        public:
            SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklog);
            virtual void launch () = 0;
            ListeningSocket * getSocket();
            // void    setNonBlocking(ListeningSocket);
    };

}

#endif