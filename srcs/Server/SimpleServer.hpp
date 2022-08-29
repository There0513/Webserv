/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: threiss <threiss@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 16:02:46 by cmarteau          #+#    #+#             */
/*   Updated: 2022/08/27 14:55:02 by threiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SimpleServer_HPP
# define SimpleServer_HPP

#include <stdio.h>
#include <unistd.h>
#include "../Sockets/SimpleSocket.hpp"
#include "../Sockets/BindingSocket.hpp"
#include "../Sockets/ListeningSocket.hpp"
#include "../Sockets/ConnectingSocket.hpp"
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
            // virtual void responder(std::string content, std::string contentType);

        public:
            SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklog);
            virtual void launch() = 0;
            ListeningSocket * getSocket();
    };
}

#endif
