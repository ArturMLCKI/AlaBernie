#pragma once

#include <string>

class Product {
    private:
        std::string name;
        std::string price;
        std::string url;
        std::string description;
        std::string image_url;
        std::string shop;
        std::string date;
    
    public:
        Product(); // Pusty konstruktor
        Product(const std::string& name, const std::string& price, const std::string& url,
                const std::string& description, const std::string& image_url, const std::string& shop, const std::string& date);
    
        // Gettery i settery
        void setName(const std::string& name);
        std::string getName() const;
    
        void setPrice(const std::string& price);
        std::string getPrice() const;
    
        void setUrl(const std::string& url);
        std::string getUrl() const;
    
        void setDescription(const std::string& description);
        std::string getDescription() const;
    
        void setImageUrl(const std::string& image_url);
        std::string getImageUrl() const;
        
        void setShop(const std::string& shop);  // Dodaj metodę setShop
        std::string getShop() const;
    
        void setDate(const std::string& date);  // Dodaj metodę setDate
        std::string getDate() const;
    };
