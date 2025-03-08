#include "WebScraper.h"
#include <iostream>
#include <regex>

size_t WebScraper::WriteCallback(void* contents, size_t size, size_t nmemb, std:: string* userp) {
    userp ->append((char*)contents, size * nmemb);
    return size *nmemb;
}

WebScraper::WebScraper() {
    // Init libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Domyślne ustawienia
    this ->user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36";
    this ->timeout = 30;
}

WebScraper::~WebScraper() {
    // Sprzątanie libcurl
    curl_global_cleanup();
}

void WebScraper::setUserAgent(const std::string& agent) {
    this->user_agent = agent;
}

void WebScraper::setTimeout(int seconds)  {
    this->timeout = seconds;
}

bool WebScraper::fetchWebpage(const std::string& url, std::string& response) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Błąd inicjalizacji CURL" << std::endl;
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL,  url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, this->user_agent.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, this->timeout);
    
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        std::cerr << "Błąd podczas pobierania strony: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    
    return true;

}

std::vector<std::string> WebScraper::parseSitemapXML(const std::string& xml_content) {
    std::vector<std::string> urls;
    std::regex url_regex("<loc>(.*?)</loc>");

    auto urls_begin = std::sregex_iterator(xml_content.begin(), xml_content.end(), url_regex);
    auto urls_end = std::sregex_iterator();

    for (std::sregex_iterator i = urls_begin; i != urls_end; ++i) {

        std::smatch match = *i;
        std::string url = match.str(1);
        urls.push_back(url);
    }

    return urls;
}

bool WebScraper::downloadSitemapAndGetProductUrls(const std::string& sitemap_url, std::vector<std::string>& product_urls) {
    std::string sitemap_content;
    if (!fetchWebpage(sitemap_url, sitemap_content)){
        std::cerr << "Nie udało się pobrać sitemap.xml z: " << sitemap_url << std::endl;
        return false;
    }

    product_urls = parseSitemapXML(sitemap_content);
    std::cout << "Pobrano " << product_urls.size() << " adresów URL z sitemap.xml" << std::endl;

    return !product_urls.empty();
}