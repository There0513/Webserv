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

	    // std::vector<std::pair<std::string, std::string > >  header;
        std::string _contentType;
        // body

    	std::map<long, std::string>                         requests; // <socket, bufferstring> // needed for loop?!?

    public:
        httpRequest(std::string buffer, long socket);
        httpRequest();
        ~httpRequest();

        std::string     readFileContent();
        void            findContentType();
        void            parseRequest(std::string buffer);
        void            getFirstLine(std::string str, std::string deli);


        void        setContentType(std::string type);
        std::string getContentType();
};

#endif