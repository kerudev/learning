CC = c++
CFLAGS = -Wall -Wextra -I../include
LDFLAGS = -lglfw -lglad -lm

SRC_DIR = src
COMMON_DIR = ../common
BUILD_DIR = build

SRC = $(wildcard $(SRC_DIR)/*.cpp)
COMMON = $(wildcard $(COMMON_DIR)/*.cpp)
ALL_SOURCES = $(SRC) $(COMMON)

OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir $(ALL_SOURCES)))

BIN = $(BUILD_DIR)/main

vpath %.cpp $(SRC_DIR) $(COMMON_DIR)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN)
