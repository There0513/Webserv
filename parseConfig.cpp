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

//Check if there are any errors in the config file
std::string     ConfigFile::checkErrorConfig(void) {

    std::map<std::string, std::vector<std::string> >::iterator  it = _content.begin();

    for (; it != _content.end(); it++)
    {
        if (it->second.empty() || (it->second.size() > 1 && it->first.substr(it->first.find_last_of("/"), it->first.length())))
        std::cout << it->second[0] << std::endl;
    }
    return "rien";
}

//Check if the directive is allowed
bool    ConfigFile::checkDirective(std::string dir) {

    if (dir.find("location") != std::string::npos)
        dir.substr(dir.find_last_of("/"), dir.length());

    std::vector<std::string>    listOfDir = {"listen", "server_name", "root", "error_page", "client_max_body_size", "index", "autoindex", "cgi_pass", "authorized_methods"};

    if (std::find(listOfDir.begin(), listOfDir.end(), dir) != listOfDir.end())
        return true;
    return false;
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

        //check if line defines a section and assign the path to the section
        if (line[0] == '[') {
            _inSection = trim(line.substr(1, line.find(']') -1));
            if (!_inSection.compare("server")) 
                _inSection.assign("server" + std::to_string(++flag));
            else if (!_inSection.compare("location/")) 
                _inSection.assign("server" + std::to_string(flag) + "/" + _inSection.substr(0, _inSection.find("/")));
            else if (!strncmp(_inSection.c_str(), "location", 8)) 
                _inSection.assign("server" + std::to_string(flag) + "/" + _inSection);
            else {
                std::cout << "Config File Error: Wrong block nomination" << std::endl;
                exit(0);
            }
            continue; }

        //store the part before the '=' in _directive, and the part after in a vector
        posEqual = line.find('=');
        _directive = trim(line.substr(0, posEqual));
        if (!checkDirective(_directive)) {
            std::cout << "Config File Error: Wrong directive" << std::endl;
            exit(0);
        }
        value = trim(line.substr(posEqual + 1, line.length() - _directive.length() - 2));
        _valuesVec = fillVector(value);

        //store the pair of _directive + vector of values in the map
        _content[_inSection + '/' + _directive] = _valuesVec;
    }
    std::cout << checkErrorConfig() << std::endl; 
}

std::map<std::string, std::vector<std::string> > const & ConfigFile::getMap() const { return _content; }

std::string ConfigFile::getSection(std::string const & port, std::string const & url, std::string const & directive) {

    std::string server = findServer(port);
    std::string str;

    if (!url.compare(""))
        str = server + directive; 
    else
        str = server + "location" + url + directive;
    return str;
}

std::vector<std::string> const & ConfigFile::getValue(std::string const & port, std::string const & url, std::string const & directive) {

    std::string str = getSection(port, url, directive);

    std::map<std::string, std::vector<std::string> >::const_iterator it = _content.find(str);
    
    if (it == _content.end())
        throw ValueNotFoundException();
    return it->second;
}

bool    ConfigFile::isLocalIP(std::string const & listen, std::string const & host) {

    static int specificIP = 0;
    int lislen = listen.length();
    int hostlen = host.length();
    int portPos = listen.find(":") + 1;
    int portPosHost = host.find(":") + 1;
    std::string hostPort = host.substr(portPosHost, hostlen - portPosHost);

    if (!listen.compare(host)) {
        specificIP++;
        return true;
    }
    else if (!listen.compare(portPos, lislen - portPos, hostPort)
            && specificIP == 0) 
        return true;
    return false;
}

std::string     ConfigFile::defineHost(std::string str) {

//  does the host specify both IP and port ?

//  yes
    if (str.find(":") != std::string::npos) 
        return str;

//  it does not specify the port: set the default port to 8080
    else if (str.find(".") != std::string::npos)
        return (str + ":8080");

//  it does not specify the IP: set the generic address 0.0.0.0
    else
        return ("0.0.0.0:" + str);
}

bool     ConfigFile::sameServerName(std::vector<std::string> server_name, std::string const & host) {

    std::vector<std::string>::reverse_iterator  rit = server_name.rbegin();

    for (; rit != server_name.rend(); rit++) {
        if (!host.compare(0, host.find(":"), *rit))
            return true;
    }
    return false;
}

bool     ConfigFile::isCandidateServer(std::vector<std::string> servers, std::string const & testServ) {

    std::vector<std::string>::reverse_iterator  rit = servers.rbegin();
    std::string server = testServ.substr(0, testServ.find("/"));

    for (; rit != servers.rend(); rit++) {
        if (!server.compare(*rit));
            return true;
    }
    return false;
}

std::string     ConfigFile::findServer(std::string const & host) {

    std::map<std::string, std::vector<std::string> >::iterator it = _content.begin();
    std::vector<std::string>    candidateServers;

// complete listen of the config file if information is missing
    for (; it != _content.end(); it++) 
        if (it->first.find("listen") != std::string::npos) 
            it->second[0] = defineHost(it->second[0]);

//  compare host of the request and listen of the config file and push the candidate servers in a vector
    it  = _content.begin();
    for (; it != _content.end(); it++) {

        if (isLocalIP(it->second[0], host) == true)
            candidateServers.push_back(it->first.substr(0, it->first.find("/") + 1));
    }
//  if there is only one candidate server, return it
    if (candidateServers.size() == 1)
        return candidateServers[0];

//  if there are multiple candidate servers with the same level of specificity
//  compare server names and return the first one to match IP+port+server_name
//  or the first one (default one) in case they are all the same
    if (candidateServers.size() > 1) {

        it  = _content.begin();
        for (; it != _content.end(); it++) {

            if (it->first.find("server_name") != std::string::npos) {

                if (sameServerName(it->second, host) == true
                    && isCandidateServer(candidateServers, it->first) == true)
                        return it->first.substr(0, it->first.find("/") + 1);
            }
        }
        it = _content.begin();
        for (; it != _content.end(); it++) {

            if (it->first.find("server_name") != std::string::npos)
                return it->first.substr(0, it->first.find("/") + 1);
        }
    }
    throw ServerNotFoundException();
}

std::string     ConfigFile::findPath(std::string const & port, std::string const & url) {

    try {
        return (getValue(port, url, "root")[0] + url);
    }
    catch (ConfigFile::ValueNotFoundException &e) {
        return (getValue(port, "", "root")[0] + url);
    }
}

bool            ConfigFile::isMethodAllowed(std::string const & port, std::string const & url, std::string const & method) {

    std::string str = getSection(port, url, "authorized_methods");
    
    std::map<std::string, std::vector<std::string> >::reverse_iterator it = _content.rbegin();

    for (; it != _content.rend(); it++)
        if (it->first.find(str) != std::string::npos
            && std::find(it->second.begin(), it->second.end(), method) != it->second.end())
            return true;
    return false;
}