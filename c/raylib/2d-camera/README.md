# c/raylib/2d-camera

Inspired by:
- raylib > examples > core > [2d camera](https://github.com/raysan5/raylib/blob/e140aca/examples/core/core_2d_camera.c)

In this project I learned how to:
- Install and build dependencies for a C project.
- Create a draw loop with `raylib`.
- Use the `GuiSliderBar` from `raygui`.
- Simulate gravity and friction.
- Draw text on the screen.

Dependencies:
- `raylib.h`: installed via `pacman`.
- `raygui.h`: built from source.

To build `raygui` from source:

```sh
git clone --depth 1 https://github.com/raysan5/raygui
mv raygui/src/raygui.h raygui/src/raygui.c
gcc -o build/raygui.so raygui/src/raygui.c -shared -fpic -DRAYGUI_IMPLEMENTATION -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
sudo mv raygui/src/raygui.c /usr/include/raygui.h
rm -rf raygui
```

Building a shared object is not really needed, as `#define RAYGUI_IMPLEMENTATION`
already does all the heavy work. So to get raygui.h without building:

```sh
git clone --depth 1 https://github.com/raysan5/raygui
sudo mv raygui/src/raygui.h /usr/include
rm -rf raygui
```

Compile with `make all`. Run with `./build/main`.
