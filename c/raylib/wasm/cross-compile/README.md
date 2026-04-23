# c/raylib/wasm/cross-compile

In this project I learned how to:
- Cross-compile a Raylib project to `Linux`, `Window` and `Web Assembly`.
- Compile and run C code to `i686-w64-mingw32-gcc` and `Wine`. 

## Dependencies

- [raylib.h v5.5](https://github.com/raysan5/raylib/blob/5.5/src/raylib.h)

```sh
cd /usr/local/include  # raylib copies header files inside this directory
wget https://raw.githubusercontent.com/raysan5/raylib/refs/tags/5.5/src/raylib.h
```

## Tools

gcc produces `ELF` executables. Windows uses `PE32` executables (.exe). How can
we compile our code to `PE32` in Linux? This is where `MinGW` and `Wine` come
into play. `MinGW` is a toolchain that compiles our code as an `.exe` files.
`Wine` can take `.exe` files and run them on Linux.

The last tool we need is `emsdk`, which is Emscripten's set of tools to compile
C code to WebAssembly.

Download links:
- MinGW: https://www.mingw-w64.org/downloads/
- Wine: https://gitlab.winehq.org/wine/wine/-/wikis/Download
- emsdk: https://emscripten.org/docs/getting_started/downloads.html

Repos:
- MinGW: https://sourceforge.net/p/mingw-w64/mingw-w64/ci/master/tree/
- Wine: https://gitlab.winehq.org/wine/wine
- emsdk: https://github.com/emscripten-core/emsdk

## Libraries

Now we need raylib's compiled libraries. Compile them from source:
- [Linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux)
- [Windows](https://github.com/raysan5/raylib/wiki/Working-on-Windows)
- [WebAssembly](https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5))

Or download the precompiled [assets](https://github.com/raysan5/raylib/releases/tag/5.5):

```sh
wget -P lib https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz  # pick yours
wget -P lib https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_i386.tar.gz   # pick yours
wget -P lib https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win32_mingw-w64.zip
wget -P lib https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_webassembly.zip
```

Then unzip everything:

```sh
cd lib
tar -xvzf raylib-5.5_linux_amd64.tar.gz
unzip raylib-5.5_win32_mingw-w64.zip
unzip raylib-5.5_webassembly.zip
```

Just for demonstration purposes, we will rename each library to distinguish it
from the rest. This is not the standard in raylib, so these libraries will live
inside `./lib` instead of `/usr/local/lib`.

```sh
cd lib
mv raylib-5.5_linux_amd64/lib/libraylib.a libraylib-lin.a
mv raylib-5.5_win32_mingw-w64/lib/libraylib.a libraylib-win.a
mv raylib-5.5_webassembly/lib/libraylib.a libraylib-wasm.a

rm -rf raylib-5.5_*
```

## Run and compile

Check out `Makefile` to see the build recipes for each target:
- `make linux`
- `make linux-cpp`
- `make windows`
- `make windows-cpp`
- `make wasm`
- `make wasm-cpp`

## Extras

- [Mozilla's guide to WASM](https://developer.mozilla.org/en-US/docs/WebAssembly/Guides/C_to_Wasm)
- [emcc -s flag](https://github.com/emscripten-core/emscripten/blob/main/src/settings.js)
- [raylib's WASM template](https://github.com/raysan5/raylib-game-template)
