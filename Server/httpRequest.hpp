#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include <iostream>
# include <map>
# include <vector>

class httpRequest
{   // tests
    private:
        std::string method;     // -> first line
        std::string url;        // -> first line
        std::string version;    // -> first line

	    std::vector<std::pair<std::string, std::string > >  header;
        // body

    	std::map<long, std::string>                         requests; // <socket, bufferstring> // needed for loop?!?

    public:
        httpRequest(std::string buffer, long socket);
        httpRequest();
        ~httpRequest();
};

#endif