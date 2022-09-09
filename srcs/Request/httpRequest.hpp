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

        int             isValid();

        void            handleURL();




/* SETTERS - GETTERS */

        void        setUrl(std::string url);
        std::string getUrl();

        // void        setContentType(std::string type);
        // std::string getContentType();

        void        setMethod(std::string method);
        std::string getMethod();

        void        setStatusCode(int nbr);
        int         getStatusCode();

        std::string *getHeaderValue(std::string const &key);
};

#endif