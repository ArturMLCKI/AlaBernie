// main.cpp
#include <iostream>
#include <string>
#include "WebScraper.h"
#include "ProductParser.h"
#include "Database.h"

int main() {
    // Konfiguracja
    std::string shop_url = "https://www.tadmar.pl/pompy-ciepla.html";
    std::string shop_name = "tadmar";
    std::string db_name = "product_prices.db";
    
    // Inicjalizacja komponentów
    WebScraper scraper;
    ProductParser parser(shop_name);
    Database db(db_name);
    
    // Inicjalizacja bazy danych
    if (!db.initialize()) {
        std::cerr << "Błąd inicjalizacji bazy danych." << std::endl;
        return 1;
    }
    
    // Pobranie strony
    std::string response;
    if (!scraper.fetchWebpage(shop_url, response)) {
        std::cerr << "Błąd pobierania strony." << std::endl;
        return 1;
    }
    
    // Parsowanie produktów
    std::vector<Product> products = parser.parseProducts(response);
    
    std::cout << "Znaleziono " << products.size() << " produktów." << std::endl;
    
    // Zapisanie produktów do bazy danych
    if (!db.saveProducts(products, shop_name)) {
        std::cerr << "Błąd podczas zapisywania produktów do bazy danych." << std::endl;
        return 1;
    }
    
    std::cout << "Produkty zostały zapisane do bazy danych." << std::endl;
    
    // Opcjonalnie: wyświetl produkty
    for (const auto& product : products) {
        std::cout << "Nazwa: " << product.getName() << std::endl;
        std::cout << "Cena: " << product.getPrice() << std::endl;
        std::cout << "URL: " << product.getUrl() << std::endl;
        std::cout << "------------------------" << std::endl;
    }
    
    return 0;
}