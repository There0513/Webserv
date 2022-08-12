#include "SimpleServer.hpp"

HDE::SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklog) {
    
    socket = new ListeningSocket(domain, service, protocol, port, interface, bklog);
}

HDE::ListeningSocket * HDE::SimpleServer::getSocket() { return socket; }