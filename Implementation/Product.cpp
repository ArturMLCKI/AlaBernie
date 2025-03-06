// Product.cpp
#include "Product.h"

Product::Product() {
    // Pusty konstruktor
}

Product::Product(const std::string& name, const std::string& price, const std::string& url) {
    this->name = name;
    this->price = price;
    this->url = url;
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