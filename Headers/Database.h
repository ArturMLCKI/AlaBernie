#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>
#include "Product.h"

class Database {
private:
    sqlite3* db;
    std::string db_name;

public:
    Database(const std::string& db_name);
    ~Database();

    // Inicializacja
    bool initialize();

    // Zapisywanie
    bool saveProduct(const Product& product, const std::string& shop_name);
    bool saveProducts(const std::vector<Product>& products, const std::string& shop_name);

    // Metody do odczytu
    std::vector<Product> getProductsByShop(const std::string& shop_name);
    std::vector<Product> getProductsHistory(const std::string& product_name);
};