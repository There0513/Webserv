#ifndef SimpleSocket_HPP
# define SimpleSocket_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

namespace HDE
{
    class SimpleSocket {
        
        private:
            struct sockaddr_in address;
            int sock;
            int connection;

        public:
        
            //Constructor
            SimpleSocket(int domain, int service, int protocol, int port, u_long interface);

            //Virtual function
            virtual int connectToNetwork(int sock, struct sockaddr_in address) = 0;

            void testConnection(int);
            struct sockaddr_in getaddress();
            int getsock();
            int getconnection();

            //Setter function
            void setconnection(int);
    };
}

#endif 