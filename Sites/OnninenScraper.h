#pragma once

#include "Database.h"
#include <string>

// Deklaracja funkcji scrapeOnninen
bool scrapeOnninen(Database& db, const std::string& sitemap_url = "https://onninen.pl/sitemap_products_001.xml");
