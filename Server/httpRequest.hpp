#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include <iostream>
# include <map>
# include <vector>

class httpRequest
{
    private:
        // method    -> first line
        // url       -> first line
        // version   -> first line

	    std::vector<std::pair<std::string, std::string > >  header;
        // body

    	std::map<long, std::string>                         requests; // <socket, bufferstring>

    public:
        httpRequest(std::string buffer, long socket);
        ~httpRequest();
};

#endif