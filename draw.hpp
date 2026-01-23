#pragma once
#include "constant.hpp"
#include "raylib.h"
#include <math.h>
#include <queue>

class tetromino; // forward declaration

inline void drawSquare(float x, float y, float size, Color color,bool transparent = false) {
    // DrawRectangleGradientH(x, y, size, size, color, ColorBrightness(color, -0.3f));
    // DrawRectangleLines(x, y, size, size, BLACK);
    if(transparent)BeginBlendMode(BLEND_ADDITIVE);
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
    if(transparent)EndBlendMode();

}

inline void drawSquareInGrid(int i, int j, Color color, bool transparent) {
    drawSquare(GRID_X_OFFSET + (j-1)*TILE_SIZE, GRID_Y_OFFSET + (i-1)*TILE_SIZE, TILE_SIZE, color,transparent);
}

inline void drawGameOver() {
    int effet3D = 3;

    Vector2 SizeGameOver = MeasureTextEx(GetFontDefault(), "Game\nOver", 90, 10);
    DrawTextEx(GetFontDefault(),"Game\nOver", Vector2{GRID_X_OFFSET + GRID_WIDTH/2 - SizeGameOver.x/2 + effet3D,
                                                    GRID_Y_OFFSET + GRID_HEIGHT/2 - SizeGameOver.y/2 - effet3D}, 90, 10, MAROON);
    DrawTextEx(GetFontDefault(),"Game\nOver", Vector2{GRID_X_OFFSET + GRID_WIDTH/2 - SizeGameOver.x/2,
                                                    GRID_Y_OFFSET + GRID_HEIGHT/2 - SizeGameOver.y/2}, 90, 10, PINK);

    Vector2 SizePlay = MeasureTextEx(GetFontDefault(), "Press space to play", 20, 2);
    Vector2 PositionPlay = Vector2{GRID_X_OFFSET + GRID_WIDTH/2 - SizePlay.x/2,
                                                    GRID_Y_OFFSET + GRID_HEIGHT/2 + SizeGameOver.y/2 + 10};
    DrawRectangleV(PositionPlay, SizePlay, BLACK);
    DrawTextEx(GetFontDefault(),"Press space to play", PositionPlay, 20, 2, PINK);
}

inline void drawBackground(int score, int level, std::queue<char> futureTetrominos, float backgroundTimer); // Forward declaration only

// Include tetrominos.hpp after class definitions to implement drawBackground
#include "tetrominos.hpp"

// Implementation of drawBackground after tetromino type is complete
inline void drawBackground(int score, int level, std::queue<char> futureTetrominos, float backgroundTimer) {
    DrawRectangleGradientH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorBrightness(PINK, -0.7f), ColorBrightness(PURPLE, -0.7f));
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
            DrawRectangle(startX + i * a, startY + j * a, a - 1, a - 1, BLACK);
        }
    }

    // Score and level
    DrawTextEx(GetFontDefault(), TextFormat("Score: %d", score), Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 20}, 20, 2, LIGHTGRAY);
    DrawTextEx(GetFontDefault(), TextFormat("Level: %d", level), Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 40}, 20, 2, LIGHTGRAY);

    // Next tetromino
    DrawTextEx(GetFontDefault(), "Next:", Vector2{GRID_X_OFFSET + GRID_WIDTH + 20, GRID_Y_OFFSET + 80}, 20, 2, LIGHTGRAY);

    char shape = futureTetrominos.front();
    tetromino nextTetromino(shape);
    Color color = charToColor(shapeToChar(nextTetromino.getShape()));
    for (auto cell : nextTetromino.getCells()) {
        int i = cell.i;
        int j = cell.j;
        drawSquare(GRID_X_OFFSET + GRID_WIDTH + 20 + j * TILE_SIZE, GRID_Y_OFFSET + 110 + i * TILE_SIZE, TILE_SIZE, color);
    }
}