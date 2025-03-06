#pragma once

#include <string>
#include <vector>
#include <curl/curl.h>

class WebScraper {
private:
    std::string user_agent;
    int timeout;
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

public:
    WebScraper();
    ~WebScraper();

    // Metody konfiguracyjne
    void setUserAgent(const std::string& agent);
    void setTimeout(int seconds);

    bool fetchWebpage(const std::string& url, std::string& response);
}; 
