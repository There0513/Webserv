#include "parseConfig.hpp"

int main(int ac, char **av) {

    if (ac != 2) {
        std::cout << "ERROR: Wrong number of arguments\n" << std::endl; 
        return (1); 
    }

    std::string path = av[1]; 
    ConfigFile  cf(path);

    std::vector<std::string> port = cf.getValue("server", "listen");
    std::cout << "port = " << port[0] << std::endl;
    
    std::vector<std::string> server_name = cf.getValue("server", "server_name");
    std:: cout << "server_name = " << server_name[1] << std::endl;

    std::vector<std::string> root = cf.getValue("server", "root");
    std:: cout << "root = " << root[0] << std::endl;

    std::vector<std::string> error_page = cf.getValue("server", "error_page");
    std::cout << "error_page = " << error_page[0] << std::endl;

    std::vector<std::string> methods = cf.getValue("location/", "authorized_methods");
    std::cout << "authorized_methods = " << methods[0] << std::endl;

    return (0);
}