#include "Product.h"

Product::Product() {
    // Pusty konstruktor
}

Product::Product(const std::string& name, const std::string& price, const std::string& url,
                 const std::string& description, const std::string& image_url, const std::string& shop, const std::string& date) {
    this->name = name;
    this->price = price;
    this->url = url;
    this->description = description;
    this->image_url = image_url;
    this->shop = shop;
    this->date = date;
}

void Product::setName(const std::string& name) {
    this->name = name;
}

std::string Product::getName() const {
    return this->name;
}

void Product::setPrice(const std::string& price) {
    this->price = price;
}

std::string Product::getPrice() const {
    return this->price;
}

void Product::setUrl(const std::string& url) {
    this->url = url;
}

std::string Product::getUrl() const {
    return this->url;
}

void Product::setDescription(const std::string& description) {
    this->description = description;
}

std::string Product::getDescription() const {
    return this->description;
}

void Product::setImageUrl(const std::string& image_url) {
    this->image_url = image_url;
}

std::string Product::getImageUrl() const {
    return this->image_url;
}

void Product::setShop(const std::string& shop) {  // Implementacja metody setShop
    this->shop = shop;
}

std::string Product::getShop() const {
    return this->shop;
}

void Product::setDate(const std::string& date) {  // Implementacja metody setDate
    this->date = date;
}

std::string Product::getDate() const {
    return this->date;
}
