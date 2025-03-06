// Database.cpp
#include "Database.h"
#include <iostream>

Database::Database(const std::string& db_name) {
    this->db_name = db_name;
    this->db = nullptr;
}

Database::~Database() {
    if (this->db) {
        sqlite3_close(this->db);
    }
}

bool Database::initialize() {
    int rc = sqlite3_open(this->db_name.c_str(), &this->db);
    
    if (rc) {
        std::cerr << "Nie udało się otworzyć bazy danych: " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
    
    // Tworzenie tabeli produktów
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "price TEXT NOT NULL,"
        "url TEXT,"
        "description TEXT,"
        "image_url TEXT,"
        "shop TEXT NOT NULL,"
        "date TEXT NOT NULL"
        ");";
    
    char* err_msg = nullptr;
    rc = sqlite3_exec(this->db, create_table_sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Błąd SQL: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }
    
    return true;
}

bool Database::saveProduct(const Product& product, const std::string& shop_name) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO products (name, price, url, description, image_url, shop, date) VALUES (?, ?, ?, ?, ?, ?, date('now'));";
    
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Błąd przygotowania SQL: " << sqlite3_errmsg(this->db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, product.getName().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, product.getPrice().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, product.getUrl().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, product.getDescription().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, product.getImageUrl().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, shop_name.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Błąd wykonania SQL: " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool Database::saveProducts(const std::vector<Product>& products, const std::string& shop_name) {
    // Rozpoczęcie transakcji dla lepszej wydajności
    char* err_msg = nullptr;
    int rc = sqlite3_exec(this->db, "BEGIN TRANSACTION;", 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Błąd rozpoczęcia transakcji: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }
    
    bool success = true;
    for (const auto& product : products) {
        if (!saveProduct(product, shop_name)) {
            success = false;
            break;
        }
    }
    
    // Zakończenie transakcji
    if (success) {
        rc = sqlite3_exec(this->db, "COMMIT;", 0, 0, &err_msg);
    } else {
        rc = sqlite3_exec(this->db, "ROLLBACK;", 0, 0, &err_msg);
    }
    
    if (rc != SQLITE_OK) {
        std::cerr << "Błąd zakończenia transakcji: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }
    
    return success;
}

std::vector<Product> Database::getProductsByShop(const std::string& shop_name) {
    std::vector<Product> products;
    sqlite3_stmt* stmt;
    
    const char* sql = "SELECT name, price, url, description, image_url FROM products WHERE shop = ? ORDER BY date DESC;";
    
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Błąd przygotowania SQL: " << sqlite3_errmsg(this->db) << std::endl;
        return products;
    }
    
    sqlite3_bind_text(stmt, 1, shop_name.c_str(), -1, SQLITE_STATIC);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Product product;
        product.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        product.setPrice(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        
        // Dodanie pozostałych właściwości, jeśli są dostępne
        if (sqlite3_column_text(stmt, 2))
            product.setUrl(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        
        if (sqlite3_column_text(stmt, 3))
            product.setDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        
        if (sqlite3_column_text(stmt, 4))
            product.setImageUrl(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        
        products.push_back(product);
    }
    
    sqlite3_finalize(stmt);
    return products;
}

std::vector<Product> Database::getProductsHistory(const std::string& product_name) {
    std::vector<Product> products;
    sqlite3_stmt* stmt;
    
    const char* sql = "SELECT name, price, url, description, image_url, shop, date FROM products WHERE name = ? ORDER BY date;";
    
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Błąd przygotowania SQL: " << sqlite3_errmsg(this->db) << std::endl;
        return products;
    }
    
    sqlite3_bind_text(stmt, 1, product_name.c_str(), -1, SQLITE_STATIC);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Product product;
        product.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        product.setPrice(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        
        if (sqlite3_column_text(stmt, 2))
            product.setUrl(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        
        if (sqlite3_column_text(stmt, 3))
            product.setDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        
        if (sqlite3_column_text(stmt, 4))
            product.setImageUrl(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        
        products.push_back(product);
    }
    
    sqlite3_finalize(stmt);
    return products;
}