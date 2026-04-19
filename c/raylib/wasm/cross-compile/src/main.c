#include <raylib.h>

#define TEXT_MARGIN_TOP  10
#define TEXT_MARGIN_LEFT 10
#define FONT_SIZE        20

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "Cross-platform compilation");
    SetTargetFPS(60);

    Rectangle box = (Rectangle) { 0, 0, 80, 60 };
    int boxSpeedX = 3;
    int boxSpeedY = 2;

    #ifdef __EMSCRIPTEN__
    char *text = "Running on Web!";
    #elif defined(_WIN32)
    char *text = "Running on Windows!";
    #elif defined (__GNUC__)
    char *text = "Running on Unix!";
    #endif

    int textW = MeasureText(text, FONT_SIZE);
    int textH = FONT_SIZE;

    SetWindowMinSize(
        textW + TEXT_MARGIN_TOP  * 2 + box.width  * 2,
        textH + TEXT_MARGIN_LEFT * 2 + box.height * 2
    );

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleRec(box, RED);

        if (((box.x + box.width)  >= GetScreenWidth())  || (box.x <= 0)) boxSpeedX *= -1;
        if (((box.y + box.height) >= GetScreenHeight()) || (box.y <= 0)) boxSpeedY *= -1;

        box.x -= boxSpeedX;
        box.y -= boxSpeedY;

        DrawText(text, 10, 10, FONT_SIZE, LIGHTGRAY);

        EndDrawing();
    }

    return 0;
}
