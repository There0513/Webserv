/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: threiss <threiss@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 16:05:16 by cmarteau          #+#    #+#             */
/*   Updated: 2022/09/20 21:26:51 by threiss          ###   ########.fr       */
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

/* Examples of how to use the Config Class for Theresa <3 *//*
    try {

        std::cout << "Path to files: " << cf.findPath("example.org:8080", "/app") << std::endl;
        std::cout << "Path to files: " << cf.findPath("example.org:8080", "/uploads") << std::endl;

        std::cout << "Is GET allowed for localhost:8080/uploads ? " << cf.isMethodAllowed("localhost:8080", "/uploads", "GET") << std::endl;

        std::vector<std::string> host = cf.getValue("localhost:8080", "", "listen"); // if the value you are looking for is not inside a location block (url of the request = /), just input "" in the URL
        std::cout << "host = " << host[0] << std::endl; // 2nd part of the map pair is always a vector, but for some values such as host or root, there can be only one element in the vector 
    
        std::vector<std::string> server_name = cf.getValue("localhost:8080", "", "server_name");
        std:: cout << "server_name = " << server_name[1] << std::endl;

        std::vector<std::string> root = cf.getValue("localhost:8080", "/uploads", "root");
        std:: cout << "root = " << root[0] << std::endl;

        std::vector<std::string> error_page = cf.getValue("localhost:8080", "" , "error_page");
        std::cout << "error_page = " << error_page[0] << std::endl;

        std::vector<std::string> methods = cf.getValue("localhost:8080", "/app", "authorized_methods");
        std::cout << "authorized_methods = " << methods[0] << std::endl;

        std::vector<std::string> methods1 = cf.getValue("localhost:8080", "/", "authorized_methods");
        std::cout << "authorized_methods = " << methods1[0] << std::endl;

        std::cout << "Path to files: " << cf.findPath("localhost:443", "/uploads") << std::endl;

        std::cout << "Is DELETE allowed for localhost:443/uploads ? " << cf.isMethodAllowed("localhost:443", "/uploads", "DELETE") << std::endl;
        std::cout << "Is POST allowed for localhost:443/uploads ? " << cf.isMethodAllowed("localhost:443", "/uploads", "POST") << std::endl;

        std::vector<std::string> root443 = cf.getValue("localhost:443", "/uploads", "root");
        std:: cout << "root = " << root443[0] << std::endl;

        std::vector<std::string> methods443 = cf.getValue("localhost:8080", "/app", "authorized_methods");
        std::cout << "authorized_methods = " << methods443[1] << std::endl;

        std::vector<std::string> upload = cf.getValue("localhost:443", "", "upload_path");
        std::cout << "upload path = " << upload[0] << std::endl;

        std::vector<std::string> redir = cf.getValue("localhost:443", "/uploads", "redirection");
        std::cout << "redir = " << redir[0] << std::endl;

        std::vector<std::string> ghost = cf.getValue("localhost:8080", "/appa", "authorized_methods");
        std::cout << "authorized_methods = " << ghost[0] << std::endl;
    }
    catch (ConfigFile::ServerNotFoundException &e) {
        std::cout << e.what() << std::endl;
    }
    catch (ConfigFile::ValueNotFoundException &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;*/
}