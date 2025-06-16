#include <stdio.h>
#include <math.h>

#define RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include "raygui.h"

# define M_PI 3.14159265358979323846
#define MAX_BUILDINGS 100
#define GRAVITY_CONSTANT 9.81f

float calc_vel_x(float deg, float friction, float mass) {
    float rad = deg * (M_PI / 180.0f);

    float px = mass * GRAVITY_CONSTANT * sinf(rad);
    float fric = friction * mass * GRAVITY_CONSTANT * sinf(rad);

    return px - fric;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

    Rectangle player = { 400, 280, 40, 40 };
    Rectangle buildings[MAX_BUILDINGS] = { 0 };
    Color buildColors[MAX_BUILDINGS] = { 0 };

    int spacing = 0;

    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        buildings[i].width = (float)GetRandomValue(50, 200);
        buildings[i].height = (float)GetRandomValue(100, 800);
        buildings[i].y = screenHeight - 130.0f - buildings[i].height;
        buildings[i].x = -6000.0f + spacing;

        spacing += (int)buildings[i].width;

        buildColors[i] = (Color){
            (unsigned char)GetRandomValue(200, 240),
            (unsigned char)GetRandomValue(200, 240),
            (unsigned char)GetRandomValue(200, 250),
            255};
    }

    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.x + 20.0f, player.y + 20.0f };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    float friction_coefficient = 0.0f;
    float mass = 1.0f;
    float vel_x = 0.0f;

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Player movement
        if (IsKeyDown(KEY_RIGHT)) player.x += 2;
        else if (IsKeyDown(KEY_LEFT)) player.x -= 2;

        vel_x = calc_vel_x(camera.rotation, friction_coefficient, mass);
        player.x += vel_x;

        // Camera target follows player
        camera.target = (Vector2){ player.x + 20, player.y + 20 };

        // Camera rotation controls
        if (IsKeyDown(KEY_A)) camera.rotation--;
        else if (IsKeyDown(KEY_D)) camera.rotation++;

        // Limit camera rotation to 90 degrees (-45 to 45)
        if (camera.rotation > 45) camera.rotation = 45;
        else if (camera.rotation < -45) camera.rotation = -45;

        // Camera zoom controls
        // Uses log scaling to provide consistent zoom speed
        camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove()*0.1f));

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;
            friction_coefficient = 0.0f;
            mass = 1.0f;
            vel_x = 0.0f;
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

                DrawRectangle(-6000, 320, 13000, 8000, DARKGRAY);

                for (int i = 0; i < MAX_BUILDINGS; i++) DrawRectangleRec(buildings[i], buildColors[i]);

                DrawRectangleRec(player, RED);

                DrawLine((int)camera.target.x, -screenHeight*10, (int)camera.target.x, screenHeight*10, GREEN);
                DrawLine(-screenWidth*10, (int)camera.target.y, screenWidth*10, (int)camera.target.y, GREEN);

            EndMode2D();

            GuiSliderBar (
                (Rectangle) { 580, 50, 200, 25 },
                "Friction",
                NULL,
                &friction_coefficient,
                0.0f,
                1.0f
            );

            GuiSliderBar (
                (Rectangle) { 580, 70, 200, 25 },
                "Mass",
                NULL,
                &mass,
                1.0f,
                10.0f
            );

            DrawText("SCREEN AREA", 640, 10, 20, RED);
            DrawFPS ( 550, 10 );

            DrawRectangle(0, 0, screenWidth, 5, RED);
            DrawRectangle(0, 5, 5, screenHeight - 10, RED);
            DrawRectangle(screenWidth - 5, 5, 5, screenHeight - 10, RED);
            DrawRectangle(0, screenHeight - 5, screenWidth, 5, RED);

            DrawRectangle( 10, 10, 250, 193, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( 10, 10, 250, 193, BLUE);

            char rotation_buf[64];
            sprintf(rotation_buf, "Deg : %.2f", camera.rotation);

            char zoom_buf[64];
            sprintf(zoom_buf, "Zoom: %.2f", camera.zoom);

            char vel_x_buf[64];
            sprintf(vel_x_buf, "VelX: %.2f", vel_x);

            char friction_buf[64];
            sprintf(friction_buf, "Fric: %.2f", friction_coefficient);

            char mass_buf[64];
            sprintf(mass_buf, "Mass: %.2f", mass);

            DrawText("Free 2d camera controls:",        20, 20, 10, BLACK);
            DrawText("- Right/Left to move Offset",     40, 40, 10, DARKGRAY);
            DrawText("- Mouse Wheel to Zoom in-out",    40, 60, 10, DARKGRAY);
            DrawText("- A / D to Rotate",               40, 80, 10, DARKGRAY);
            DrawText("- R to reset Zoom and Rotation",  40, 100, 10, DARKGRAY);

            DrawText(rotation_buf,                      40, 140, 10, DARKGRAY);
            DrawText(zoom_buf,                          40, 160, 10, DARKGRAY);

            DrawText(vel_x_buf,                         140, 140, 10, DARKGRAY);
            DrawText(friction_buf,                      140, 160, 10, DARKGRAY);
            DrawText(mass_buf,                          140, 180, 10, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
