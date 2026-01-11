#include<raylib.h>
#include<iostream>

#include "grid.hpp"
#include "tetrominos.hpp"
#include "timer.hpp"

int main(void)
{
    Grid grid;
    grid.print();

    // Initialization
    //--------------------------------------------------------------------------------------
    // const int screenWidth = 300;
    // const int screenHeight = 600;
    // const int tileSize = 30;

    bool grounded = false;
    bool pred_pressed = false;
    tetromino_T t;
    RepeatingTimer fallTimer(1.0f);

    // 1 case : 30x30 pixels
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");

    if (!IsWindowReady()) {
        std::cerr << "Failed to initialize window (display unavailable). Exiting." << std::endl;
        return 1;
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        fallTimer.Update();
        if (fallTimer.Trigger()) {
            grounded = t.fall(grid);
            if (grounded) {
                grid.placeTetromino(t);
                grid.removeFilledRows();
                t = tetromino_T();
            }
        } // Make the tetromino fall every second
        if(IsKeyDown(KEY_RIGHT) && !pred_pressed){t.moveRight(grid);}
        if(IsKeyDown(KEY_LEFT) && !pred_pressed){t.moveLeft(grid);}
        if(IsKeyDown(KEY_DOWN) && !pred_pressed){fallTimer.SkipToNextTrigger();}
        if(IsKeyDown(KEY_UP) && !pred_pressed){t.rotateClockwise(grid);}
        pred_pressed = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_UP);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            grid.draw();
            t.draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 