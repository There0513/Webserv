#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include <iostream>
# include <map>
# include <vector>
#include "../Config/parseConfig.hpp"

class httpRequest
{   // tests
    private:
        std::string _method;     // -> first line
        std::string _url;        // -> first line
        std::string _version;    // -> first line

        std::string _host;      // -> host from body request

	    std::vector<std::pair<std::string, std::string > >  _header;
        // std::string _contentType;
        std::string _body;

        int         _statusCode;
        bool        _isChunked;


    	std::map<long, std::string>                         requests; // <socket, bufferstring> // needed for loop?!?

    public:
        httpRequest(std::string buffer, long socket);
        httpRequest();
        ~httpRequest();

        std::string     readFileContent();
        void            findContentType();
        void            parseRequest(std::string buffer);
        void            getFirstLine(std::string str, std::string deli);
        int             checkFirstLine();
        void            parseHeader(std::string buffer);
        void            parseBody();

        int             isValid(ConfigFile & cf);

        void            handleURL();


/* SETTERS - GETTERS */

        void        setUrl(std::string url);
        std::string getUrl();

        void        setHost(std::string buffer);
        std::string getHost();

        // void        setContentType(std::string type);
        // std::string getContentType();

        void        setMethod(std::string method);
        std::string getMethod();


        std::string *getHeaderValue(std::string const &key);
};

#endif