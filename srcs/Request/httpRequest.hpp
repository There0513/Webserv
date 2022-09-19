#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include <iostream>
# include <map>
# include <vector>
#include "../Config/parseConfig.hpp"

class httpRequest
{
    private:
        std::string     _method;     // -> first line
        std::string     _url;        // -> first line
        std::string     _version;    // -> first line

	    std::vector<std::pair<std::string, std::string > >  _header;
        std::string     _host;      // -> host from body request
        std::string     _query;
        std::string     _body;
        
        ConfigFile*     _ConfigFile;

        int             _statusCode;
        bool            _isChunked;

        std::string     _extension;


    public:
        bool        _auto;// tmp public
        httpRequest(std::string buffer, long socket);
        httpRequest();
        ~httpRequest();

        std::string     readContent();
        std::string     readFileContent();
        std::string     readDirectoryAutoindex();
        void            findContentType();
        void            parseRequest(std::string buffer);
        void            getFirstLine(std::string str, std::string deli);
        int             checkFirstLine();
        void            setQuery();
        void            parseHeader(std::string buffer);
        void            parseBody();

        int             isValid(ConfigFile & cf);
        void            handleURL(ConfigFile & cf);
        bool            isCgi = false;

/* SETTERS - GETTERS */

        void        setUrl(std::string url);
        std::string getUrl();
        
        void        setHost(std::string buffer);
        std::string getHost();

        std::string getBody();
        std::string getExtension();

        void        setMethod(std::string method);
        std::string getMethod();

        void        setStatusCode(int nbr);
        int         getStatusCode();

        std::string *getHeaderValue(std::string const &key);
        void        setHeaderValue(std::string key, std::string value);

        ConfigFile* getConfigFile();
        
};

#endif