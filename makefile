# Compiler et options
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Répertoires
SRC_DIR = src
BUILD_DIR = obj
BIN_DIR = bin

# Fichiers sources et objets
SOURCES = $(wildcard $(SRC_DIR)/**/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Nom de l'exécutable
TARGET = $(BIN_DIR)/main

# Règle par défaut
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des fichiers objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phonies
.PHONY: all clean