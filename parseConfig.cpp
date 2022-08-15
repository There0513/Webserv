#include "parseConfig.hpp"
#include <fstream>
#include <algorithm>
#include <cstring>

//trim tabs, spaces and \n
std::string trim(std::string const & source, char const *delims = "\t\r\n") {

    std::string result(source);
    std::string::size_type  index = result.find_last_of(delims);

    if (index != std::string::npos)
        result.erase(++index);
    index = result.find_first_not_of(delims);
    if (index != std::string::npos)
        result.erase(0, index);
    else
        result.erase();

    return result;
}

//tokenize the string inside value by | and ; 
//in case there are several values for the _directive
//store them in a vector
std::vector<std::string>    fillVector(std::string const & value) {

    std::vector<std::string> vector;
    char                     val[value.length() + 1];
    char *                   token;

    strcpy(val, value.c_str());
    token = std::strtok(val, "|;");
    while (token) {
            std::string str = token;
            vector.push_back(str);
            token = std::strtok(NULL, "|;");}
    return vector;
}


ConfigFile::ConfigFile(std::string const & configFile) {

    std::ifstream           file(configFile.c_str());
        
    std::string             line;
    std::string             value;
    
    int                     posEqual;
    int                     flag = 0;

    while (std::getline(file, line)) {

        //remove whitespaces
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

        //skip line if it is empty or commented
        if (line.empty() || line[0] == '#' || line[0] == '}')
            continue;

        //check if line defines a section 
        if (line[0] == '[') {
            _inSection = trim(line.substr(1, line.find(']') -1));
            if (!_inSection.compare("server")) //assign a number to the server to be differentiate if there are several
                _inSection.assign("server" + std::to_string(++flag));
            if (!strncmp(_inSection.c_str(), "location", 8)) { // build the location path vector 
                _locationPath.push_back("server" + std::to_string(flag) + "/" + _inSection);
                _inSection.assign("server" + std::to_string(flag) + "/location");
            }
            continue; }

        //store the part before the '=' in _directive, and the part after in a vector
        posEqual = line.find('=');
        _directive = trim(line.substr(0, posEqual));
        value = trim(line.substr(posEqual + 1, line.length() - _directive.length() - 2));
        _valuesVec = fillVector(value);

        //store the pair of _directive + vector of values in the map
        _content[_inSection + '/' + _directive] = _valuesVec;
    }
}

std::map<std::string, std::vector<std::string> > const & ConfigFile::getMap() const { return _content; }

std::vector<std::string> const & ConfigFile::getValue(std::string const & section, std::string const & entry) const {

    std::map<std::string, std::vector<std::string> >::const_iterator it = _content.find(section + '/' + entry);
    if (it == _content.end())
        throw "does not exists";
    return it->second;
}

std::string     ConfigFile::findServer(std::string const & port) {

    std::map<std::string, std::vector<std::string> >::reverse_iterator it = _content.rbegin();

    for (; it != _content.rend(); it++)
        if (it->first.find("listen") != std::string::npos 
            && !port.compare(it->second[0]))
            return it->first.substr(0, it->first.find("/"));
    throw "Port not found";
}

std::string     ConfigFile::findUrl(std::string const & port, std::string const & url) {

    std::string server = findServer(port);
    std::string str = server + "/location" + url;

    if (std::find(_locationPath.begin(), _locationPath.end(), str) != _locationPath.end())
        return _content[server + "/location" + "/root"][0];
    throw "url not found";
}