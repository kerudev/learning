CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lGL -lglut

SRC_DIR = src
BUILD_DIR = build

BIN = $(BUILD_DIR)/tetrahedron

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(BIN)

clean:
	rm -f $(BUILD_DIR)/*.o $(BIN)
