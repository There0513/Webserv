/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: threiss <threiss@studend.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 16:01:48 by cmarteau          #+#    #+#             */
/*   Updated: 2022/08/30 07:26:37 by threiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SimpleServer.hpp"

HDE::SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklog) {
    
    socket = new ListeningSocket(domain, service, protocol, port, interface, bklog);
}

HDE::ListeningSocket * HDE::SimpleServer::getSocket() { return socket; }