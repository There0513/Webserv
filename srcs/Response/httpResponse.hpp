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

		char        **_envVar;      // env var for cgi

    
    public:
        char        **execArgv;     // argument for execve(execArgv[0], execArgv, environ)  tmp public
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
        int         executeCgi();
        void        handleCgiFile();
        void        createEnvVar();

        void        setContentType(std::string type);
        std::string getContentType();
};

#endif