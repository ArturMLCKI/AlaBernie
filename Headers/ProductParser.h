#pragma once

#include <string>
#include <vector>
#include <regex>
#include "Product.h"

class ProductParser {
private:
    std::string shop_name;
    std::regex product_regex;
    std::regex name_regex;
    std::regex price_regex;
    std::regex url_regex;

public:
    ProductParser(const std::string& shop_name);

    void configureForShop(const std::string& shop_name);

    std::vector<Product> parseProducts(const std::string& html);
};

