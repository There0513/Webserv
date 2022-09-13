#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include <iostream>
# include <map>
# include <vector>

class httpRequest
{   // tests
    private:
        std::string _method;     // -> first line
        std::string _url;        // -> first line
        std::string _version;    // -> first line

        std::string _query;
	    std::vector<std::pair<std::string, std::string > >  _header;
        std::string _body;

        int         _statusCode;
        bool        _isChunked;


    	std::map<long, std::string>                         requests; // <socket, bufferstring> // needed for loop?!?

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

        int             isValid();

        void            handleURL();




/* SETTERS - GETTERS */

        void        setUrl(std::string url);
        std::string getUrl();

        std::string getBody();

        void        setMethod(std::string method);
        std::string getMethod();

        void        setStatusCode(int nbr);
        int         getStatusCode();

        std::string *getHeaderValue(std::string const &key);
        void        setHeaderValue(std::string key, std::string value);
};

#endif