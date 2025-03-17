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

// ONNINEN START

std::vector<Product> ProductParser::parseOnninen(const std::string& html) {
    std::vector<Product> products;
    
    // Wyrażenie regularne do wyciągnięcia podstawowych informacji o produkcie
    std::regex product_name_regex("<h1[^>]*class=\"[^\"]*foDHzh[^\"]*\"[^>]*>(.*?)</h1>");
    std::regex product_price_regex("<span[^>]*class=\"[^\"]*main-price[^\"]*\"[^>]*>([0-9]+(?:,[0-9]{1,2})?) zł</span>");
    std::regex product_desc_regex("<div[^>]*class=\"[^\"]*description[^\"]*\"[^>]*>(.*?)</div>");
    std::regex product_img_regex("<img[^>]*src=\"([^\"]+)\"[^>]*>");
    
    // Wyciągnięcie danych
    Product product;
    
    // Nazwa produktu
    std::smatch name_match;
    if (std::regex_search(html, name_match, product_name_regex)) {
        std::string name = name_match.str(1);
        // Usunięcie tagów HTML
        name = std::regex_replace(name, std::regex("<[^>]*>"), "");
        // Usunięcie nadmiarowych białych znaków
        name = std::regex_replace(name, std::regex("\\s+"), " ");
        name = std::regex_replace(name, std::regex("^\\s+|\\s+$"), "");
        product.setName(name);
    }
    
    // Cena produktu
    std::smatch price_match;
    if (std::regex_search(html, price_match, product_price_regex)) {
        std::string price = price_match.str(1);
        // Usunięcie tagów HTML
        price = std::regex_replace(price, std::regex("<[^>]*>"), "");
        // Usunięcie nadmiarowych białych znaków
        price = std::regex_replace(price, std::regex("\\s+"), " ");
        price = std::regex_replace(price, std::regex("^\\s+|\\s+$"), "");
        product.setPrice(price);
    }
    
    // Opis produktu
    std::smatch desc_match;
    if (std::regex_search(html, desc_match, product_desc_regex)) {
        std::string description = desc_match.str(1);
        // Usunięcie tagów HTML
        description = std::regex_replace(description, std::regex("<[^>]*>"), "");
        // Usunięcie nadmiarowych białych znaków
        description = std::regex_replace(description, std::regex("\\s+"), " ");
        description = std::regex_replace(description, std::regex("^\\s+|\\s+$"), "");
        product.setDescription(description);
    }
    
    // URL obrazka 
    std::smatch img_match;
    if (std::regex_search(html, img_match, product_img_regex)) {
        product.setImageUrl(img_match.str(1));
    }
    
    // URL produktu jest przekazywany z zewnątrz
    
    if (!product.getName().empty() && !product.getPrice().empty()) {
        products.push_back(product);
    }
    
    return products;
}

// ONNINEN END