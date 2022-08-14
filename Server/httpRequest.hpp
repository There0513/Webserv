#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include <iostream>

class httpRequest
{
    private:
        // method    -> first line
        // url       -> first line
        // version   -> first line

	    // std::vector<std::pair<std::string, std::string > > header;
        // body

    public:
        httpRequest(std::string buffer);
        ~httpRequest();
};

#endif