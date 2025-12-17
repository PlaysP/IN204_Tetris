#include<iostream>
#include<raylib.h>
#include "grid.hpp"

int main(void)
{
    Grid grid;
    grid.print();

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 300;
    const int screenHeight = 600;
    const int tileSize = 30;
    // 1 case : 30x30 pixels

    InitWindow(screenWidth, screenHeight, "Tetris");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            grid.draw(tileSize);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 