/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarteau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 16:02:31 by cmarteau          #+#    #+#             */
/*   Updated: 2022/08/20 16:02:32 by cmarteau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef testServer_HPP
# define testServer_HPP

#include "SimpleServer.hpp"

namespace HDE {

    class testServer : public SimpleServer {

        private:
            char    buffer[30000] = {0};
            int     newSocket;
            void    accepter();
            void    handler();
            void    responder();

            int    connectList[10]; // array of connected sockets
            fd_set  socks; // socket fds we want to wake up for, using select()
            int     highSocket; // highest fd

        public:
            testServer();
            void    launch();
            void    buildSelectList(void);
            void    handle_new_connections();
            void    setNonBlocking(int);
            void    deal_with_data(int);
    };
}

#endif
