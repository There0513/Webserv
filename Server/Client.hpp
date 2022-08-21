#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "testServer.hpp"

class Client {
    private:
        int         _socket;
        int         _code;
        long        _ret;    // return of read()
        // char        _buffer[4096];    // char - otherwise error in getBuffer

    public:
        std::string _buffer[30000];
        Client();
        ~Client();

        int     reading();


        void    setSocket(int socket);
        int     getSocket();
        std::string getBuffer();
};

#endif