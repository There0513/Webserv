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
        std::vector<std::string>                            _locationPath; // A vector containing the paths associated to the location directives
        std::string                                         _directive; // A string containing the directive
        std::vector<std::string>                            _valuesVec; // A vector containing the possible values
        std::string                                         _inSection; // A string containing the section 

    public:
            //Constructor
            ConfigFile(std::string const & configFile);

            //Getters
            std::map<std::string, std::vector<std::string> > const & getMap() const;
            std::vector<std::string> const & getValue(std::string const & section, std::string const & entry) const;

            std::string     findServer(std::string const & port); // A function to find the first server with the port corresponding to the request
            std::string     findUrl(std::string const & port, std::string const & url); // A function to check if the given URL exists, and if yes return the associated root
            bool            isMethodAllowed(std::string method); // A function to check if the required method is allowed
};

#endif

