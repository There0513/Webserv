/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: threiss <threiss@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 16:02:31 by cmarteau          #+#    #+#             */
/*   Updated: 2022/08/27 16:28:23 by threiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef testServer_HPP
# define testServer_HPP

#include <stdio.h>
#include "SimpleServer.hpp"
#include "../Client/Client.hpp"
#include "../Request/httpRequest.hpp"

namespace HDE {

    class testServer : public SimpleServer {

        private:
            char    buffer[30000] = {0};
            int     newSocket;
            int     _ret; // tmp?! return of read()
            void    accepter();
            void    handler();  // tmp theresa:  // => methodHandler() in response ?!
            void    responder();
            void    handleResponse(std::string content, std::string contentType, int connectListSocket);
            void    responder(std::string content, std::string contentType);

            // std::map<int, Client*>  clients;


            int     connectList[10]; // array of connected sockets (clients)
            fd_set  socks; // socket fds we want to wake up for, using select()
            int     highSocket; // highest fd

            void    buildSelectList(void);
            void    handle_new_connections();
            void    setNonBlocking(int);
            void    deal_with_data(int);
            
        public:
            testServer(int port = 8080);
            void    launch();

            // tmp:
            int     readingTmp();
    };
}

#endif
