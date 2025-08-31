CC = c++
CFLAGS = -Wall -Wextra -I../include
LDFLAGS = -L../lib -lcommon -lglfw -lglad -lm

SRC_DIR = src
BUILD_DIR = build

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

BIN = $(BUILD_DIR)/main

all: $(BIN)

../lib/libcommon.a:
	$(MAKE) -C ../

$(BIN): $(OBJ) ../lib/libcommon.a
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN)
