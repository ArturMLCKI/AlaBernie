#include "Database.h"
#include "OnninenScraper.h"
#include "WebScraper.h"
#include "ProductParser.h"
#include <iostream>

int main() {
    // Inicjalizacja bazy danych
    std::string db_name = "products.db";  // Nazwa pliku bazy danych
    Database db(db_name);

    // Próba inicjalizacji bazy danych
    if (!db.initialize()) {
        std::cerr << "Nie udało się zainicjować bazy danych!" << std::endl;
        return 1;
    }

    // Rozpoczęcie procesu pobierania i zapisywania produktów
    if (scrapeOnninen(db)) {
        std::cout << "Zakończono proces pobierania produktów." << std::endl;
    } else {
        std::cerr << "Wystąpił problem podczas pobierania produktów." << std::endl;
        return 1;
    }

    return 0;
}
