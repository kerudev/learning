CC = gcc
CFLAGS = -Wall -Wextra -I../libs
LDFLAGS = -lglfw -lglad -lm

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = ../libs

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

LIB_SRC = $(wildcard $(LIB_DIR)/*.c)
LIB_OBJ = $(patsubst $(LIB_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_SRC))

BIN = $(BUILD_DIR)/main

all: $(BIN)

$(BIN): $(OBJ) $(LIB_OBJ)
	$(CC) $(OBJ) $(LIB_OBJ) -o $(BIN) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/*.o $(BIN)
