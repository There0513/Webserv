#ifndef parseConfig_HPP
# define parseConfig_HPP

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>

class ConfigFile {

    private:
        //A map that will contain the directive (ex. port, location, root, server_name) 
        //and a vector containing the possible values (ex. if there are several server_names)
        std::map<std::string, std::vector<std::string> >    _content;
        std::string                                         _directive; // A string containing the directive
        std::vector<std::string>                            _valuesVec; // A vector containing the possible values
        std::string                                         _inSection; // A string containing the section 

        std::string     getSection(std::string const & port, std::string const & url, std::string const & directive); // A function that returns the Section path where the value is
        std::string     findServer(std::string const & host); // A function to find the first server with the port corresponding to the request

        std::string     defineHost(std::string str);
        bool            isLocalIP(std::string const & listen, std::string const & host); // A function to check if the IP address of the request matches our local IP
        bool            sameServerName(std::vector<std::string> server_name, std::string const & host);
        bool            isCandidateServer(std::vector<std::string> servers, std::string const & testServ);


    public:
            //Constructor
            ConfigFile(std::string const & configFile);

            //Getters
            std::map<std::string, std::vector<std::string> > const & getMap() const;
            std::vector<std::string> const &                         getValue(std::string const & port, std::string const & url, std::string const & directive);

            std::string     findPath(std::string const & port, std::string const & url); // A function to check if the given URL exists, and if yes return the associated root
            bool            isMethodAllowed(std::string const & port, std::string const & url, std::string const & method); // A function to check if the required method is allowed

};

#endif

