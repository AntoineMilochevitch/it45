CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC_DIR = .
BUILD_DIR = ../../obj/algo_little
BIN_DIR = ../../bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS = $(OBJECTS:.o=.d)

TARGET = $(BIN_DIR)/little

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(TARGET)

.PHONY: all clean
