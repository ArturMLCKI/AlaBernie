// ProductParser.cpp
#include "ProductParser.h"
#include <iostream>

ProductParser::ProductParser(const std::string& shop_name) {
    this->shop_name = shop_name;
    configureForShop(shop_name);
}

void ProductParser::configureForShop(const std::string& shop_name) {
    // Konfiguracja wyrażeń regularnych dla różnych sklepów
    if (shop_name == "example_shop") {
        // Wyrażenia regularne dopasowane do struktury HTML przykładowego sklepu
        // Użycie std::regex_constants::ECMAScript | std::regex_constants::icase zamiast dotall
        this->product_regex = std::regex("<div class=\"product\">(.*?)</div>", 
                                  std::regex_constants::ECMAScript | std::regex_constants::icase);
        this->name_regex = std::regex("<h2>(.*?)</h2>");
        this->price_regex = std::regex("<span class=\"price\">(.*?)</span>");
        this->url_regex = std::regex("<a href=\"(.*?)\">");
    }
    else if (shop_name == "other_shop") {
        // Wyrażenia regularne dla innego sklepu
        this->product_regex = std::regex("<article class=\"product-item\">(.*?)</article>", 
                                  std::regex_constants::ECMAScript | std::regex_constants::icase);
        this->name_regex = std::regex("<h3 class=\"product-name\">(.*?)</h3>");
        this->price_regex = std::regex("<div class=\"price\">(.*?)</div>");
        this->url_regex = std::regex("<a class=\"product-link\" href=\"(.*?)\">");
    }
    else {
        std::cerr << "Nieznany sklep: " << shop_name << ". Używanie domyślnych wyrażeń regularnych." << std::endl;
        // Domyślne wyrażenia regularne
        this->product_regex = std::regex("<div class=\"product\">(.*?)</div>", 
                                  std::regex_constants::ECMAScript | std::regex_constants::icase);
        this->name_regex = std::regex("<h2>(.*?)</h2>");
        this->price_regex = std::regex("<span class=\"price\">(.*?)</span>");
        this->url_regex = std::regex("<a href=\"(.*?)\">");
    }
}

std::vector<Product> ProductParser::parseProducts(const std::string& html) {
    std::vector<Product> products;
    
    auto products_begin = std::sregex_iterator(html.begin(), html.end(), this->product_regex);
    auto products_end = std::sregex_iterator();
    
    for (std::sregex_iterator i = products_begin; i != products_end; ++i) {
        std::smatch product_match = *i;
        std::string product_html = product_match.str(1);
        
        Product product;
        
        // Wyciągnięcie nazwy produktu
        std::smatch name_match;
        if (std::regex_search(product_html, name_match, this->name_regex)) {
            product.setName(name_match.str(1));
        }
        
        // Wyciągnięcie ceny produktu
        std::smatch price_match;
        if (std::regex_search(product_html, price_match, this->price_regex)) {
            product.setPrice(price_match.str(1));
        }
        
        // Wyciągnięcie URL produktu
        std::smatch url_match;
        if (std::regex_search(product_html, url_match, this->url_regex)) {
            product.setUrl(url_match.str(1));
        }
        
        products.push_back(product);
    }
    
    return products;
}