#pragma once
#include "constant.hpp"
#include "raylib.h"
#include <math.h>
#include <queue>

class tetromino; // forward declaration

inline void drawSquare(float x, float y, float size, Color color, bool transparent = false) {
    if (transparent) {
        Color transparentGrey = { 128, 128, 128, 150 };
        DrawRectangle(x, y, size, size, transparentGrey);
    } else {
        float effet3D = size/5;
        // Haut
        DrawTriangle(Vector2{x,y}, Vector2{x+size/2,y+size/2},Vector2{x+size,y}, ColorBrightness(color, 0.3f));
        // Gauche
        DrawTriangle(Vector2{x,y}, Vector2{x,y+size}, Vector2{x+size/2,y+size/2}, ColorBrightness(color, -0.2f));
        // Droite
        DrawTriangle(Vector2{x+size,y}, Vector2{x+size/2,y+size/2}, Vector2{x+size,y+size}, ColorBrightness(color, -0.4f));
        // Bas
        DrawTriangle(Vector2{x,y+size}, Vector2{x+size,y+size}, Vector2{x+size/2,y+size/2}, ColorBrightness(color, -0.6f));
        // Centre
        DrawRectangle(x + effet3D, y + effet3D, size - 2*effet3D, size - 2*effet3D, color);
    }
}

inline void drawSquareInGrid(int i, int j, Color color, bool transparent, bool adv) {
    int Adv_offset = adv ? 2*GRID_X_OFFSET + GRID_WIDTH + 170 : 0;
    drawSquare(GRID_X_OFFSET + Adv_offset + (j-1)*TILE_SIZE, GRID_Y_OFFSET + (i-1)*TILE_SIZE, TILE_SIZE, color, transparent);
}

inline void drawMiddle(std::string text) {
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), 40, 4);
    DrawTextEx(GetFontDefault(), text.c_str(), Vector2{SCREEN_WIDTH/2 - textSize.x/2, SCREEN_HEIGHT/2 - textSize.y/2}, 40, 4, PINK);
}

inline void drawGameOver(std::string message, std::string message_bis, bool adv) {
    int effet3D = 3;
    int Adv_offset = adv ? 2*GRID_X_OFFSET + GRID_WIDTH + 170 : 0;
    
    Vector2 SizeGameOver = MeasureTextEx(GetFontDefault(), message.c_str(), 90, 10);
    DrawTextEx(GetFontDefault(), message.c_str(), Vector2{GRID_X_OFFSET + Adv_offset + GRID_WIDTH/2 - SizeGameOver.x/2 + effet3D,
                                                    GRID_Y_OFFSET + GRID_HEIGHT/2 - SizeGameOver.y/2 - effet3D}, 90, 10, MAROON);
    DrawTextEx(GetFontDefault(), message.c_str(), Vector2{GRID_X_OFFSET + Adv_offset + GRID_WIDTH/2 - SizeGameOver.x/2,
                                                    GRID_Y_OFFSET + GRID_HEIGHT/2 - SizeGameOver.y/2}, 90, 10, PINK);

    Vector2 SizePlay = MeasureTextEx(GetFontDefault(), message_bis.c_str(), 20, 2);
    Vector2 PositionPlay = Vector2{GRID_X_OFFSET + Adv_offset + GRID_WIDTH/2 - SizePlay.x/2,
                                                    GRID_Y_OFFSET + GRID_HEIGHT/2 + SizeGameOver.y/2 + 10};
    DrawRectangleV(PositionPlay, SizePlay, BLACK);
    DrawTextEx(GetFontDefault(), message_bis.c_str(), PositionPlay, 20, 2, PINK);
}

inline void drawBackground(int score, int level, std::queue<char> futureTetrominos, float backgroundTimer, Grid& grid); // Forward declaration only

// Include tetrominos.hpp after class definitions to implement drawBackground
#include "tetrominos.hpp"

// Implementation of drawBackground after tetromino type is complete
inline void drawBackground(int score, int level, std::queue<char> futureTetrominos, float backgroundTimer, Grid& grid) {
    DrawRectangleGradientH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorBrightness(PINK, -0.5f), ColorBrightness(PURPLE, -0.5f));
    float a = 90.0f;
    float v = 15.0f;
    float rawDelta = fmod(backgroundTimer * v, a);
    int deltaPx = (int)round(rawDelta); // force integer pixel offset to avoid subpixel gaps

    // Calculate how many tiles are needed to cover the screen and add a margin
    int nx = (int)ceil((float)SCREEN_WIDTH / a) + 2;
    int ny = (int)ceil((float)SCREEN_HEIGHT / a) + 2;

    // Start offset so the pattern aligns and covers left/top edges.
    // Use -(a-1) so the first gap (1px) is at x=0 when delta==0,
    // then shift by delta to animate to the right as delta increases.
    float startX = -(a - 1) + deltaPx;
    float startY = -(a - 1) + deltaPx;

    for (int i = 0; i <= nx; ++i) {
        for (int j = 0; j <= ny; ++j) {
            DrawRectangle(startX + i * a, startY + j * a, a - 2, a - 2, BLACK);
        }
    }

    // Score and level
    DrawTextEx(GetFontDefault(), TextFormat("Score: %d", score), Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 20}, 20, 2, LIGHTGRAY);
    DrawTextEx(GetFontDefault(), TextFormat("Level: %d", level), Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 40}, 20, 2, LIGHTGRAY);

    // Next tetromino
    DrawTextEx(GetFontDefault(), "Next:", Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 80}, 20, 2, LIGHTGRAY);

    char shape = futureTetrominos.front();
    tetromino nextTetromino(shape, grid);
    Color color = charToColor(shapeToChar(nextTetromino.getShape()));
    for (auto cell : nextTetromino.getCells()) {
        int i = cell.i;
        int j = cell.j;
        drawSquare(GRID_X_OFFSET + GRID_WIDTH + 20 + j * TILE_SIZE, GRID_Y_OFFSET + 110 + i * TILE_SIZE, TILE_SIZE, color, false);
    }

    // Controls
    DrawTextEx(GetFontDefault(), "LEFT/RIGHT\n Move", Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 250}, 20, 2, LIGHTGRAY);
    DrawTextEx(GetFontDefault(), "UP\n Rotate", Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 310}, 20, 2, LIGHTGRAY);
    DrawTextEx(GetFontDefault(), "DOWN\n Fast Fall", Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 370}, 20, 2, LIGHTGRAY);
    DrawTextEx(GetFontDefault(), "SPACE\n Drop", Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 430}, 20, 2, LIGHTGRAY);
    DrawTextEx(GetFontDefault(), "ENTER\n Enable/Disable\n Shadow", Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 490}, 20, 2, LIGHTGRAY);
}

inline void drawAskIP(char* ipAddress) {
    std::string prompt = "Enter Server IP Address:";
    Vector2 promptSize = MeasureTextEx(GetFontDefault(), prompt.c_str(), 30, 4);
    DrawTextEx(GetFontDefault(), prompt.c_str(), Vector2{SCREEN_WIDTH / 2 - promptSize.x / 2, SCREEN_HEIGHT / 2 - 60}, 30, 4, PINK);

    std::string displayIP = std::string(ipAddress) + "_";
    Vector2 ipSize = MeasureTextEx(GetFontDefault(), displayIP.c_str(), 30, 4);
    DrawTextEx(GetFontDefault(), displayIP.c_str(), Vector2{SCREEN_WIDTH / 2 - ipSize.x / 2, SCREEN_HEIGHT / 2}, 30, 4, RED);

    std::string instructions = "Press ENTER to connect";
    Vector2 instrSize = MeasureTextEx(GetFontDefault(), instructions.c_str(), 20, 2);
    DrawTextEx(GetFontDefault(), instructions.c_str(), Vector2{SCREEN_WIDTH / 2 - instrSize.x / 2, SCREEN_HEIGHT / 2 + 60}, 20, 2, PINK);
}