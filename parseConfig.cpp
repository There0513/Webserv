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
//in case there are several values for the directive
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
    std::vector<std::string> vector;
    std::string             line;
    std::string             directive;
    std::string             value;
    std::string             inSection;
    int                     posEqual;
    int                     posEnd;

    while (std::getline(file, line)) {

        //remove whitespaces
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

        //skip line if it is empty or commented
        if (line.empty() || line[0] == '#' || line[0] == '}')
            continue;

        //check if line defines a section
        if (line[0] == '[') {
            inSection = trim(line.substr(1, line.find(']') -1));
            continue; }

        //store the part before the '=' in directive, and the part after in a vector
        posEqual = line.find('=');
        directive = trim(line.substr(0, posEqual));
        value = trim(line.substr(posEqual + 1, line.length() - directive.length() - 2));
        vector = fillVector(value);

        //store the pair of directive + vector of values in the map
        _content[inSection + '/' + directive] = vector;
    }
}

std::map<std::string, std::vector<std::string> > const & ConfigFile::getMap() const { return _content; }

std::vector<std::string> const & ConfigFile::getValue(std::string const & section, std::string const & entry) const {

    std::map<std::string, std::vector<std::string> >::const_iterator it = _content.find(section + '/' + entry);
    if (it == _content.end())
        throw "does not exists";
    return it->second;
}