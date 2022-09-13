/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: threiss <threiss@studend.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 16:02:31 by cmarteau          #+#    #+#             */
/*   Updated: 2022/09/13 18:08:00 by threiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef testServer_HPP
# define testServer_HPP

#include <stdio.h>
#include "SimpleServer.hpp"
#include "../Request/httpRequest.hpp"
#include "../Config/parseConfig.hpp"

namespace HDE {

    class testServer : public SimpleServer {

        private:
            char    buffer[30000] = {0};
            int     newSocket;
            int     _ret;
            void    accepter();
            void    handler();
            void    responder();
            void    handleResponse(std::string content, std::string contentType, int connectListSocket);
            void    responder(std::string content, std::string contentType);

            int     connectList[10]; // array of connected sockets (clients)
            fd_set  socks; // socket fds we want to wake up for, using select()
            int     highSocket; // highest fd

            void    buildSelectList(void);
            void    handle_new_connections(ListeningSocket *socketToHandle);
            void    setNonBlocking(int);
            void    deal_with_data(int);
            
        public:
            testServer(ConfigFile cf);
            
            ConfigFile* _ConfigFile; // Configuration file defined in the main
            void        launch();
    };
}

#endif
