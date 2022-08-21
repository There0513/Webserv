#include "Client.hpp"

Client::Client() {}

Client::~Client() {}


// read the clients request:
int     Client::reading() {
    _ret = read(_socket, _buffer, 30000); // read or recv
    if (_ret <= 0) {
        close(_socket);
        // FD_CLR(_socket, &fd_set); //      FD_CLR() removes a given file descriptor from a set (https://linux.die.net/man/3/fd_clr) fd_set is a fixed size buffer
        // handle error (INTERNAL_SERVER_ERROR = 500)
            // generate response with error page with or without auto
        // erase _socket from sockets-map and restart loop through socket-map
        return -1;
    }
    _buffer[_ret] = '\0';
    
    return 1;
}


void        Client::setSocket(int socket) {
    _socket = socket;
}
int         Client::getSocket() {
    return _socket;
}

// std::string Client::getBuffer() {
//     return _buffer;
// }