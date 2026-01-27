#pragma once
#include "raylib.h"

#define TILE_SIZE 30
#define NB_COLS 10
#define NB_ROWS 20
#define GRID_WIDTH (TILE_SIZE * NB_COLS)
#define GRID_HEIGHT (TILE_SIZE * NB_ROWS)

#define GRID_X_OFFSET 30
#define GRID_Y_OFFSET 30

#define SCREEN_WIDTH (GRID_WIDTH + 2* GRID_X_OFFSET + 170)
#define SCREEN_HEIGHT (GRID_HEIGHT + 2* GRID_Y_OFFSET)

#define ADDRESS_PORT 1233

inline Color charToColor(char c) {
    if (c=='r') return RED;
    if (c=='y') return GOLD;
    if (c=='b') return SKYBLUE;
    if (c=='d') return DARKBLUE;
    if (c=='g') return LIME;
    if (c=='o') return ORANGE;
    if (c=='v') return VIOLET;
    return BEIGE;
}
inline char shapeToChar(char shape) {
    if (shape=='O') return 'r';
    if (shape=='I') return 'b';
    if (shape=='J') return 'd';
    if (shape=='L') return 'o';
    if (shape=='S') return 'g';
    if (shape=='Z') return 'v';
    if (shape=='T') return 'y';
    return ' ';
}