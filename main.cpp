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

    KeyPressTimer keyRight(0.2f, KEY_RIGHT);
    KeyPressTimer keyLeft(0.2f, KEY_LEFT);
    KeyPressTimer keyDown(0.2f, KEY_DOWN);
    KeyPressTimer keyUp(0.2f, KEY_UP);
    KeyPressTimer keySpace(0.2f, KEY_SPACE);

    tetromino_T t;
    RepeatingTimer fallTimer(1.0f);

    // 1 case : 30x30 pixels
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");

    if (!IsWindowReady()) {
        std::cerr << "Failed to initialize window (display unavailable). Exiting." << std::endl;
        return 1;
    }
    printf("Window initialized successfully.\n");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop

    bool gameOver = false;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
    if (gameOver) {
        BeginDrawing();
        drawGameOver();
        EndDrawing();

        if (keySpace.IsPressedAndReady()) {
            // Reset game
            grid.reset();
            t = tetromino_T();
            grounded = false;
            gameOver = false;
        }
        keySpace.Update();

    } else {
        fallTimer.Update();
        if (fallTimer.Trigger()) {
            grounded = t.fall(grid);
            if (grounded) {
                grid.placeTetromino(t);
                grid.removeFilledRows();

                // Check for game over (after removing filled rows !)
                if (grid.GameOver()){
                    gameOver = true;
                    continue;
                }
                
                // New tetromino
                t = tetromino_T();
            }
        } // Make the tetromino fall every second
        if(keyRight.IsPressedAndReady()){t.moveRight(grid);}
        if(keyLeft.IsPressedAndReady()){t.moveLeft(grid);}
        if(keyDown.IsPressedAndReady()){fallTimer.SkipToNextTrigger();}
        if(keyUp.IsPressedAndReady()){t.rotateClockwise(grid);}

        keyDown.Update();
        keyLeft.Update();
        keyRight.Update();
        keyUp.Update();
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            grid.draw();
            t.draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    

    return 0;
} 