#ifndef parseConfig_HPP
# define parseConfig_HPP

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>

class ConfigFile {

    private:
        //A map that will contain the key as a string (ex. port, location, root, server_name) 
        //and a vector containing the possible options (ex. if there are several server_names)
        std::map<std::string, std::vector<std::string> > _content;

    public:
            //Constructor
            ConfigFile(std::string const & configFile);

            //Getters
            std::map<std::string, std::vector<std::string> > const & getMap() const;
            std::vector<std::string> const & getValue(std::string const & section, std::string const & entry) const;
};

#endif

