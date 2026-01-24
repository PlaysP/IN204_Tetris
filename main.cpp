#include<raylib.h>
#include<iostream>
#include <random>
#include "grid.hpp"
#include "tetrominos.hpp"
#include "timer.hpp"
#include <queue>


int updateScore(int rowsRemoved, int score, int level);
int updateLevel(int level, int* rowsRemovedCounter);
char randomTetromino();
void tetrominoIsGrounded();

float speed[21] = {
    53.0/60.0f, 49.0/60.0f, 45.0/60.0f, 41.0/60.0f, 37.0/60.0f,
    33.0/60.0f, 28.0/60.0f, 22.0/60.0f, 17.0/60.0f, 11.0/60.0f,
    10.0/60.0f, 9.0/60.0f, 8.0/60.0f, 7.0/60.0f, 6.0/60.0f,
    6.0/60.0f, 5.0/60.0f, 5.0/60.0f, 4.0/60.0f, 4.0/60.0f, 3.0/60.0f
};

std::queue<char> futureTetrominos;
Grid grid;
tetromino t = tetromino('T', grid);
int score = 0;
int level = 0;
bool EnableBotPos = true;
int rowsRemovedOnce;
int rowsRemovedCounter = 0;
RepeatingTimer fallTimer(speed[level]);
int main(void)
{
    grid.print();

    float backgroundTimer = 0.0f;

    bool fastfall = false;
    bool grounded = false;

    futureTetrominos.push(randomTetromino());
    futureTetrominos.push(randomTetromino());

    KeyPressTimer keyRight(0.15f, KEY_RIGHT);
    KeyPressTimer keyLeft(0.15f, KEY_LEFT);
    KeyPressTimer keyDown(0.15f, KEY_DOWN);
    KeyPressTimer keyUp(0.20f, KEY_UP);
    KeyPressTimer keySpace(0.20f, KEY_SPACE);
    KeyPressTimer keyEnter(0.20f, KEY_ENTER);

    t = tetromino(futureTetrominos.front(), grid);
    futureTetrominos.pop();
    futureTetrominos.push(randomTetromino());

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

        ClearBackground(BLACK);
        drawBackground(score, level, futureTetrominos, backgroundTimer, grid);
        grid.draw();
        drawGameOver();
        
        EndDrawing();

        if (keySpace.IsPressedAndReady()) {
            // Reset game
            grid.reset();

            t = tetromino(futureTetrominos.front(), grid);
            futureTetrominos.pop();
            futureTetrominos.push(randomTetromino());

            grounded = false;
            gameOver = false;
            score = 0;
            level = 0;
            rowsRemovedCounter = 0;
            fallTimer.SetInterval(speed[level]);
        }
        keySpace.Update();

    } else {
        fallTimer.Update();
        if (fallTimer.Trigger() || fastfall) {
            if (fastfall) {
                fastfall = false;
                fallTimer.Reset();
            }
            grounded = t.fall(grid);
            if (grounded) {
                tetrominoIsGrounded();
                if (grid.GameOver()){
                    gameOver = true;
                    continue;
                }
            }
        } // Make the tetromino fall every second
        if(keySpace.IsPressedAndReady()){t.fastFall(); fastfall = true;}
        if(keyRight.IsPressedAndReady()){t.moveRight(grid);}
        if(keyLeft.IsPressedAndReady()){t.moveLeft(grid);}
        if(keyDown.IsPressedAndReady()){fallTimer.SkipToNextTrigger();}
        if(keyUp.IsPressedAndReady()){t.rotateClockwise(grid);}
        if(keyEnter.IsPressedAndReady()){EnableBotPos = !EnableBotPos;}

        keyDown.Update();
        keyLeft.Update();
        keyRight.Update();
        keyUp.Update();
        keySpace.Update();
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            drawBackground(score, level, futureTetrominos, backgroundTimer, grid);
            grid.draw();
            t.draw(EnableBotPos);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    backgroundTimer += GetFrameTime();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    

    return 0;
} 

void tetrominoIsGrounded(){
    grid.placeTetromino(t);
    rowsRemovedOnce = grid.removeFilledRows();
    rowsRemovedCounter += rowsRemovedOnce;

    level = updateLevel(level,&rowsRemovedCounter);
    score = updateScore(rowsRemovedOnce, score, level);
    if(level <= 20) fallTimer.SetInterval(speed[level]);

    // New tetromino
    t = tetromino(futureTetrominos.front(), grid);
    futureTetrominos.pop();
    futureTetrominos.push(randomTetromino());
}

char shapes[] = {'T', 'O', 'I', 'J', 'L', 'S', 'Z'};

char randomTetromino(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 6);

    return shapes[dist(gen)];
}



int updateLevel(int level, int* rowsRemovedCounter){
    if(*rowsRemovedCounter >=10){
        *rowsRemovedCounter -=10;
        level++;
    }
    return level;
}


int updateScore(int rowsRemoved,int score, int level){
    int points;
    switch(rowsRemoved) {
        case 0:
            points = 0;
            break;
        case 1:
            points = 40 * (level + 1);
            break;
        case 2:
            points= 100 * (level + 1);
            break;
        case 3:
            points = 300 * (level + 1);
            break;
        default:
            points = 1200 * (level + 1);
            break;
    }

    score += points;

    return score;
}