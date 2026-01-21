#pragma once
#include "constant.hpp"
#include "raylib.h"
#include <queue>

class tetromino; // forward declaration

inline void drawSquare(int x, int y, int size, Color color) {
    DrawRectangleGradientH(x, y, size, size, color, ColorBrightness(color, -0.3f));
    DrawRectangleLines(x, y, size, size, BLACK);
}

inline void drawSquareInGrid(int i, int j, Color color) {
    drawSquare(GRID_X_OFFSET + (j-1)*TILE_SIZE, GRID_Y_OFFSET + (i-1)*TILE_SIZE, TILE_SIZE, color);
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

inline void drawBackground(int score, int level, std::queue<char> futureTetrominos); // Forward declaration only

// Include tetrominos.hpp after class definitions to implement drawBackground
#include "tetrominos.hpp"

// Implementation of drawBackground after tetromino type is complete
inline void drawBackground(int score, int level, std::queue<char> futureTetrominos) {
    ClearBackground(BLACK);

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