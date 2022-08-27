#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include <iostream>
# include <map>
# include <vector>

class   httpResponse
{
    private:
        std::string pageContent;
        std::string _contentType;

    
    public:
        httpResponse();
        ~httpResponse();
        void        setPageContent(std::string content);
        std::string getPageContent();
        void        findContentType(std::string url);

        void        methodHandler(std::string method);


        void        setContentType(std::string type);
        std::string getContentType();
};

#endif