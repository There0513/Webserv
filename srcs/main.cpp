/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarteau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 16:05:16 by cmarteau          #+#    #+#             */
/*   Updated: 2022/08/24 16:05:17 by cmarteau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server/testServer.hpp"
#include "./Config/parseConfig.hpp"

int main(int ac, char **av) {

    if (ac != 2) {
    
        std::cout << "ERROR: Wrong number of arguments\n" << std::endl; 
        return (1); 
    }

    std::string path = av[1]; 
    ConfigFile  cf(path);

    std::map<std::string, std::vector<std::string> > mapConfig = cf.getMap();
    std::map<std::string, std::vector<std::string> >::iterator it = mapConfig.begin();

    int count = 0;

    for (; it != mapConfig.end(); it++) {

        if (it->first.find("listen") != std::string::npos) {
            std::cout << it->second[0] << std::endl;
            count++;
            std::cout << count << std::endl;
        }
    }
}

