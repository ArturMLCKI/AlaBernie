CC = g++
CFLAGS = -Wall -std=c++11 -IHeaders
LDFLAGS = -lcurl -lsqlite3
BUILD_DIR = build

SOURCES = main.cpp Implementation/WebScraper.cpp Implementation/Product.cpp Implementation/ProductParser.cpp Implementation/Database.cpp
OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
EXECUTABLE = web_scraper

# Domyślna reguła
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Kompilacja plików .cpp do .o
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Czyszczenie plików .o i binarki
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

# Uruchamianie programu po kompilacji
run: all
	./$(EXECUTABLE)
