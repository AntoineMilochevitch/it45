# Compiler et options
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Répertoires
SRC_DIR = .
BUILD_DIR = ../../obj/algo_genetique
BIN_DIR = ../../bin

# Sources et objets
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
TABOU_OBJ = ../../obj/algo_recherche_tabou/solution.o ../../obj/algo_recherche_tabou/rechercheTabou.o

# Nom de l'exécutable
TARGET = $(BIN_DIR)/genetique

# Règle par défaut
all: $(TARGET)

# Construction de l'exécutable
$(TARGET): $(OBJECTS) $(TABOU_OBJ)
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