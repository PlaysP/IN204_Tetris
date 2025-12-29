#pragma once
#include "raylib.h"

inline void drawSquare(int i, int j, int tileSize, Color color) {
    DrawRectangleGradientH((j-1)*tileSize, (i-1)*tileSize, tileSize, tileSize, color, ColorBrightness(color, -0.3f));
    DrawRectangleLines((j-1)*tileSize, (i-1)*tileSize, tileSize, tileSize, BLACK);
}