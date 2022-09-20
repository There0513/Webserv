/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: threiss <threiss@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 16:05:16 by cmarteau          #+#    #+#             */
/*   Updated: 2022/09/13 22:34:32 by threiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server/testServer.hpp"
#include "./Config/parseConfig.hpp"

void    printMapConfig(ConfigFile cf) {
    std::map<std::string, std::vector<std::string> > mapConfig = cf.getMap();
    std::map<std::string, std::vector<std::string> >::iterator it = mapConfig.begin();

    int count = 0;

    for (; it != mapConfig.end(); it++) {
        // if (it->first.find("listen") != std::string::npos) {
            // std::cout << it->second[0] << std::endl;
            // count++;
            // std::cout << count << std::endl;
        // }
        std::cout << it->first << " = " << std::endl;
        std::vector<std::string>::iterator ite = it->second.begin();
        for (; ite != it->second.end(); ite++) {
            std::cout << "\t" << ite->c_str() << std::endl;
        }
    }
}

int main(int ac, char **av) {
    if (ac != 2) {
        std::cout << "ERROR: Wrong number of arguments\n" << std::endl;
        std::cout << "Usage: ./webserv [Config File]" << std::endl;        
        return (1); 
    }
    std::string path = av[1]; 
    ConfigFile  cf(path);
    // printMapConfig(cf);
    HDE::testServer t(cf);

    std::cout << std::endl;
}