CC = c++
CFLAGS  = -Wall -Wextra -Iinclude

COMMON_DIR = common
INCLUDE_DIR = include
BUILD_DIR  = build
LIB_DIR    = lib

SRC_COMMON = $(wildcard $(COMMON_DIR)/*.cpp) $(wildcard $(INCLUDE_DIR)/*.hpp)
OBJ_COMMON = $(patsubst $(COMMON_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_COMMON))
LIB_COMMON = $(LIB_DIR)/libcommon.a

all: $(LIB_COMMON)

$(LIB_COMMON): $(OBJ_COMMON)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(COMMON_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)
