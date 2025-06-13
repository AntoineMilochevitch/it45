# Compiler et options
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Répertoires
SRC_DIR = .
BUILD_DIR = ../../obj/algo_little
BIN_DIR = ../../bin

# Sources et objets
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Nom de l'exécutable
TARGET = $(BIN_DIR)/little

# Règle par défaut
all: $(TARGET)

# Construction de l'exécutable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des fichiers objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

# Phonies
.PHONY: all clean