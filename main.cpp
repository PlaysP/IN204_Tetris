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
    const int screenWidth = 300;
    const int screenHeight = 600;
    const int tileSize = 30;

    bool grounded = false;
    bool pred_pressed = false;
    tetromino t = tetromino('O', Vector(6,8));
    RepeatingTimer fallTimer(1.0f);

    // 1 case : 30x30 pixels
    InitWindow(screenWidth, screenHeight, "Tetris");

    if (!IsWindowReady()) {
        std::cerr << "Failed to initialize window (display unavailable). Exiting." << std::endl;
        return 1;
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    std::cout << "Truc\n";
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        fallTimer.Update();
        if (fallTimer.Trigger()){grounded = t.fall(tileSize,grid);} // Make the tetromino fall every second
        std::cout<<pred_pressed<<"\n";
        if(IsKeyDown(KEY_RIGHT) && !pred_pressed){t.moveRight(tileSize,grid);}
        if(IsKeyDown(KEY_LEFT) && !pred_pressed){t.moveLeft(tileSize,grid);}
        pred_pressed = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            grid.draw(tileSize);
            t.draw(tileSize);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
} 