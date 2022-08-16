#include "parseConfig.hpp"

int main(int ac, char **av) {

    if (ac != 2) {
        std::cout << "ERROR: Wrong number of arguments\n" << std::endl; 
        return (1); 
    }

    std::string path = av[1]; 
    ConfigFile  cf(path);

    std::cout << cf.findPath("80", "/uploads") << std::endl;

    std::cout << cf.isMethodAllowed("80", "/uploads", "GET") << std::endl;

    std::vector<std::string> port = cf.getValue("80", "", "listen");
    std::cout << "port = " << port[0] << std::endl;
    
    std::vector<std::string> server_name = cf.getValue("80", "", "server_name");
    std:: cout << "server_name = " << server_name[1] << std::endl;

    std::vector<std::string> root = cf.getValue("80", "/uploads", "root");
    std:: cout << "root = " << root[0] << std::endl;

    std::vector<std::string> error_page = cf.getValue("80", "" , "error_page");
    std::cout << "error_page = " << error_page[0] << std::endl;

    std::vector<std::string> methods = cf.getValue("80", "/app", "authorized_methods");
    std::cout << "authorized_methods = " << methods[0] << std::endl;

    std::vector<std::string> methods1 = cf.getValue("80", "/", "authorized_methods");
    std::cout << "authorized_methods = " << methods1[0] << std::endl;


    return (0);
}