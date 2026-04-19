SRC_DIR = src
BUILD_DIR = build

BIN = $(BUILD_DIR)/main

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

default:
	@echo "Run with 'make linux', 'make windows' or 'make wasm'"

linux:
	@mkdir -p $(BUILD_DIR)
	gcc src/main.c -o build/main \
		-Llib -lraylib-lin \
		-lm
	./build/main

windows:
	@mkdir -p $(BUILD_DIR)
	i686-w64-mingw32-gcc src/main.c -o build/main \
		-I/usr/local/include \
		-Llib -lraylib-win \
		-L/usr/local/lib -lm -lgdi32 -lwinmm
	./build/main.exe

wasm:
	@mkdir -p $(BUILD_DIR)
	emcc src/main.c -o build/index.html \
		-I/usr/local/include \
		-Llib -lraylib-wasm \
		-L/usr/local/lib -lm -lglfw \
		-s ASYNCIFY \
		-s USE_GLFW=3 \
		-DPLATFORM_WEB
	python -m http.server -d ./build

clean:
	rm -rf $(BUILD_DIR)
