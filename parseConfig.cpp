/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarteau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 00:28:11 by cmarteau          #+#    #+#             */
/*   Updated: 2022/08/20 00:28:14 by cmarteau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parseConfig.hpp"

// ================================== CONSTRUCTOR: PARSE + STORE THE ELEMENTS IN A MAP ======================================

ConfigFile::ConfigFile(std::string const & configFile) {

    std::ifstream           file(configFile.c_str());
        
    std::string             line;
    std::string             value;
    
    int                     posEqual;
    int                     flag = 0;

    while (std::getline(file, line)) {

        // REMOVE WHITESPACES
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end()); 

        // SKIP LINE IF IT IS EMPTY OR COMMENTED
        if (line.empty() || line[0] == '#' || line[0] == '}') 
            continue;

        // CHECK IF LINE DEFINES A SECTION AND ASSIGN PATH TO THE SECTION
        if (line[0] == '[') {

            _inSection = trim(line.substr(1, line.find(']') -1));

            if (!_inSection.compare("server")) 
                _inSection.assign("server" + std::to_string(++flag));

            else if (!_inSection.compare("location/")) 
                _inSection.assign("server" + std::to_string(flag) + "/" + _inSection.substr(0, _inSection.find("/")));

            else if (!strncmp(_inSection.c_str(), "location/", 9)) 
                _inSection.assign("server" + std::to_string(flag) + "/" + _inSection);

            else {

                std::cout << "Config File Error: " << _inSection << ": Wrong block nomination" << std::endl;
                exit(0);
            }
            continue; 
        }

        // STORE THE PART BEFORE THE "=" IN _DIRECTIVE AND THE PART AFTER IN _VALUESVEC
        posEqual = line.find('=');
        _directive = trim(line.substr(0, posEqual));
        if (!checkDirective(_directive)) {

            std::cout << "Config File Error: " << _directive << ": Wrong directive" << std::endl;
            exit(0);
        }
        value = trim(line.substr(posEqual + 1, line.length() - _directive.length() - 2));
        _valuesVec = fillVector(value);

        // STORE THE PAIR OF _DIRECTIVE + _VALUESVEC IN THE MAP
        _content[_inSection + '/' + _directive] = _valuesVec; 

    }
    checkErrorConfig(); 
}

// ================================== FIND THE RIGHT SERVER TO SERVE THE REQUEST ======================================


// FIND THE SERVER THAT WILL SERVE THE REQUEST
std::string     ConfigFile::findServer(std::string const & host) {

    std::map<std::string, std::vector<std::string> >::iterator it = _content.begin();
    std::vector<std::string>    candidateServers;

// COMPLETE LISTEN DIRECTIVE IF INFORMATION IS MISSING 
    for (; it != _content.end(); it++) 
        if (it->first.find("listen") != std::string::npos) 
            it->second[0] = defineHost(it->second[0]);

//  DETERMINE CANDIDATE SERVERS
    it  = _content.begin();
    for (; it != _content.end(); it++) {

        if (isLocalIP(it->second[0], host) == true)
            candidateServers.push_back(it->first.substr(0, it->first.find("/") + 1));
    }

//  RETURN SERVER IF THERE IS ONLY ONE CANDIDATE 
    if (candidateServers.size() == 1)
        return candidateServers[0];

// ELSE, COMPARE SERVER NAMES
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

// DOES THE HOST IP CORRESPOND TO A LOCAL IP ?
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

// DOES THE HOST SPECIFY BOTH IP AND PORT ?
std::string     ConfigFile::defineHost(std::string str) {

//  YES
    if (str.find(":") != std::string::npos) 
        return str;

//  DOES NOT SPECIFY THE PORT: SET THE DEFAULT PORT TO 8080
    else if (str.find(".") != std::string::npos)
        return (str + ":8080");

//  DOES NOT SPECIFY IP: SET THE GENERIC ADDRESS TO 0.0.0.0
    else
        return ("0.0.0.0:" + str);
}

// DOES THE SERVER_NAME DIRECTIVE CORRESPOND TO THE HOST OF THE REQUEST ?
bool     ConfigFile::sameServerName(std::vector<std::string> server_name, std::string const & host) {

    std::vector<std::string>::reverse_iterator  rit = server_name.rbegin();

    for (; rit != server_name.rend(); rit++) {

        if (!host.compare(0, host.find(":"), *rit))
            return true;
    }
    return false;
}

// LIST OF CANDIDATE SERVERS THAT COULD SERVE THE REQUEST 
bool     ConfigFile::isCandidateServer(std::vector<std::string> servers, std::string const & testServ) {

    std::vector<std::string>::reverse_iterator  rit = servers.rbegin();
    std::string server = testServ.substr(0, testServ.find("/"));

    for (; rit != servers.rend(); rit++) {

        if (!server.compare(*rit));
            return true;
    }
    return false;
}

// ================================== ERROR CHECK: CONFIGURATION FILE ======================================

void     ConfigFile::checkErrorConfig(void) {

    std::map<std::string, std::vector<std::string> >::iterator  it = _content.begin();

    // CHECK POSSIBLE SYNTAX ERRORS IN THE LOCATION BLOCK
    if (_content.find("//") != _content.end()) {

        std::cout << "Config File Error: Wrong syntax in the location block" << std::endl;
        exit(0);
    }
    
    for (; it != _content.end(); it++)
    {
        // CHECK NB OF VALUES IN DIRECTIVES
        if (it->second.empty() || (it->second.size() > 1 
            && it->first.substr(it->first.find_last_of("/") + 1, it->first.length()) != "authorized_methods"
            && it->first.substr(it->first.find_last_of("/") + 1, it->first.length()) != "server_name"
            && it->first.substr(it->first.find_last_of("/") + 1, it->first.length()) != "error_page"
            && it->first.substr(it->first.find_last_of("/") + 1, it->first.length()) != "cgi")
            ) {
                std::cout << "Config File Error: " << it->first << ": Too many values for directive" << std::endl;
                exit(0);
            }
        
        // CHECK AUTOINDEX
        if (it->first.find("autoindex") != std::string::npos
            && it->second[0].compare("on") && it->second[0].compare("off")) {

                std::cout << "Config File Error: " << it->first << ": autoindex cannot be interpreted" << std::endl;
                exit(0);
            }

        // CHECK PORT CONFIGURATION
        if (it->first.find("listen") != std::string::npos) {
            if (std::count(it->second[0].begin(), it->second[0].end(), ':') != 1) {

                std::cout << "Config File Error: " << it->first << ": Wrong port syntax" << std::endl;
                exit(0);
            }
        }

        // CHECK PAGES EXTENSION
        if (it->first.find("error_page") != std::string::npos) {

            if (it->second[0].substr(it->second[0].find_last_of(".") + 1) != "html"
            || it->second[1].substr(it->second[0].find_last_of(".") + 1) != "html") {

                std::cout << "Config File Error: " << it->first << ": Wrong extension" << std::endl;
                exit(0);
            }
        }
    }
    // NO CONFIGURATION ISSUE
    std::cout << "Config File: ready to be used" << std::endl;
}

// CHECK IF DIRECTIVE IS ALLOWED
bool    ConfigFile::checkDirective(std::string dir) {

    if (dir.find("location") != std::string::npos)
        dir.substr(dir.find_last_of("/"), dir.length());

    std::vector<std::string>    listOfDir = {"listen", "server_name", "root", "error_page", "client_max_body_size", "index", "autoindex", "cgi", "authorized_methods"};

    if (std::find(listOfDir.begin(), listOfDir.end(), dir) != listOfDir.end())
        return true;
    return false;
}

// ===================================================== UTILS ===========================================================

//  TRIM 
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

// TOKENIZE THE STRING BY | AND ;
std::vector<std::string>    fillVector(std::string const & value) {

    std::vector<std::string> vector;
    char                     val[value.length() + 1];
    char *                   token;

    strcpy(val, value.c_str());
    token = std::strtok(val, "|;");

    while (token) {

            std::string str = token;
            vector.push_back(str);
            token = std::strtok(NULL, "|;");
    }

    return vector;
}