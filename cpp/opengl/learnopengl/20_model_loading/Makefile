CXX = c++
CXXFLAGS = -Wall -Wextra -I../include -O3 -march=native -DNDEBUG
LDFLAGS = -L../lib -lcommon -lglad -lglfw -lassimp

SRC_DIR = src
BUILD_DIR = build

BIN = $(BUILD_DIR)/main
LIB = ../lib/libcommon.a

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

all: $(BIN)
	$(MAKE) -C ../

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN)
