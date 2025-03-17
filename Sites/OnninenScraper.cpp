#include "OnninenScraper.h"
#include "WebScraper.h"
#include "ProductParser.h"
#include <iostream>
#include <chrono>
#include <thread>

// Funkcja pomocnicza do konwersji do UTF-8
std::string encodeToUtf8(const std::string& input) {
    // W większości przypadków dane już są w UTF-8, ale mogą wymagać konwersji
    // z innego kodowania (np. ISO-8859-2, Windows-1250)
    return input;
}

bool scrapeOnninen(Database& db, const std::string& sitemap_url) {
    // Ustawienie kodowania UTF-8 w bazie danych
    db.execute("PRAGMA encoding = 'UTF-8';");

    WebScraper scraper;
    ProductParser parser("onninen");
    
    // Ustawienia user-agent oraz timeout
    scraper.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");
    scraper.setTimeout(30);
    
    // Pobieranie URLi produktów z mapy strony
    std::vector<std::string> product_urls;
    if (!scraper.downloadSitemapAndGetProductUrls(sitemap_url, product_urls)) {
        std::cerr << "Nie udało się pobrać listy produktów" << std::endl;
        return false;
    }
    
    // Ograniczenie liczby pobranych produktów
    const size_t max_products = 100;
    if (product_urls.size() > max_products) {
        std::cout << "Ograniczenie liczby produktów do " << max_products << std::endl;
        product_urls.resize(max_products);
    }
    
    int success_count = 0;
    int error_count = 0;
    
    // Przetwarzanie URLi i zapisywanie produktów
    for (size_t i = 0; i < product_urls.size(); ++i) {
        const std::string& url = product_urls[i];
        std::cout << "Przetwarzanie produktu " << (i + 1) << "/" << product_urls.size()
                  << " - " << url << std::endl;
        
        // Pobieranie strony HTML
        std::string html;
        if (!scraper.fetchWebpage(url, html)) {
            std::cerr << "Błąd pobierania strony: " << url << std::endl;
            error_count++;
            continue;
        }
        
        // Parsowanie strony HTML do produktów
        std::vector<Product> products = parser.parseOnninen(html);
        
        if (products.empty()) {
            std::cerr << "Nie udało się sparsować produktu z: " << url << std::endl;
            error_count++;
            continue;
        }
        
        // Dodanie URL do każdego produktu i upewnienie się, że dane są w UTF-8
        for (auto& product : products) {
            product.setUrl(url);
            
            // Konwersja wszystkich pól tekstowych do UTF-8
            product.setName(encodeToUtf8(product.getName()));
            product.setPrice(encodeToUtf8(product.getPrice()));
            product.setDescription(encodeToUtf8(product.getDescription()));
            product.setImageUrl(encodeToUtf8(product.getImageUrl()));
        }
        
        // Zapisanie produktów do bazy danych
        if (db.saveProducts(products, "onninen")) {
            success_count += products.size();
        } else {
            std::cerr << "Błąd zapisywania produktów do bazy danych" << std::endl;
            error_count++;
        }
        
        // Odpoczynek przed kolejnym żądaniem
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    // Podsumowanie operacji
    std::cout << "Podsumowanie: pobrano " << success_count << " produktów, "
              << error_count << " błędów" << std::endl;
    
    return (success_count > 0);
}