/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarteau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 16:01:48 by cmarteau          #+#    #+#             */
/*   Updated: 2022/08/20 16:01:50 by cmarteau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SimpleServer.hpp"

HDE::SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklog) {
    
    socket = new ListeningSocket(domain, service, protocol, port, interface, bklog);
}

HDE::ListeningSocket * HDE::SimpleServer::getSocket() { return socket; }