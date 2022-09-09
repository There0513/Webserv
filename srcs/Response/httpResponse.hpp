#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include "../Request/httpRequest.hpp"
# include <iostream>
# include <map>
# include <vector>

class   httpResponse
{
    private:
        std::string pageContent;
        std::string _contentType;

    
    public:
        httpRequest request;
        httpResponse();
        ~httpResponse();
        void        setPageContent(std::string content);
        std::string getPageContent();
        void        findContentType(std::string url);


        /* handle methods */
        void        GETMethod();
        void        POSTMethod();
        void        DELETEMethod();
        void        methodHandler(std::string method);

        /* handle CGI */
        int         checkCgi();
        void        handleCgi();

        void        setContentType(std::string type);
        std::string getContentType();
};

#endif