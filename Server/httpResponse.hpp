#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include <iostream>
# include <map>
# include <vector>

class   httpResponse
{
    private:
        std::string pageContent;
    
    public:
        httpResponse();
        ~httpResponse();
        void        setPageContent(std::string content);
        std::string getPageContent();
};

#endif