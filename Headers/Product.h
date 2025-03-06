#pragma once

#include <string>

class Product {
private:
    std::string name;
    std::string price;
    std::string url;
    std::string description;
    std::string image_url;

public:
    Product();
    Product(const std::string& name, const std::string& price, const std::string& url);

    //Gettery i settery

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
};