#pragma once
#include "constant.hpp"
#include "raylib.h"

inline void drawSquare(int i, int j, int tileSize, Color color) {
    DrawRectangleGradientH((j-1)*tileSize, (i-1)*tileSize, tileSize, tileSize, color, ColorBrightness(color, -0.3f));
    DrawRectangleLines((j-1)*tileSize, (i-1)*tileSize, tileSize, tileSize, BLACK);
}

inline void drawGameOver() {
    Vector2 SizeGameOver = MeasureTextEx(GetFontDefault(), "Game\nOver", 90, 10);
    DrawTextEx(GetFontDefault(),"Game\nOver", Vector2{SCREEN_WIDTH/2 - SizeGameOver.x/2 + 3, SCREEN_HEIGHT/2 - SizeGameOver.y/2 - 3}, 90, 10, MAROON);
    DrawTextEx(GetFontDefault(),"Game\nOver", Vector2{SCREEN_WIDTH/2 - SizeGameOver.x/2, SCREEN_HEIGHT/2 - SizeGameOver.y/2}, 90, 10, PINK);

    Vector2 SizePlay = MeasureTextEx(GetFontDefault(), "Press space to play", 20, 2);
    Vector2 PositionPlay = Vector2{SCREEN_WIDTH/2 - SizePlay.x/2, SCREEN_HEIGHT/2 + SizeGameOver.y/2 + 10};

    DrawRectangleV(PositionPlay, SizePlay, BLACK);
    DrawTextEx(GetFontDefault(),"Press space to play", PositionPlay, 20, 2, PINK);
}