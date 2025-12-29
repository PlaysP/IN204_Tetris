#include "grid.hpp"
#include "tetrominos.hpp"

Color charToColor(char c) {
    if (c=='r') return RED;
    if (c=='y') return GOLD;
    if (c=='b') return SKYBLUE;
    if (c=='d') return DARKBLUE;
    if (c=='g') return LIME;
    if (c=='o') return ORANGE;
    if (c=='v') return VIOLET;
    return BEIGE;
}

char TetrominoToChar(tetromino &t) {
    char shape = t.getShape();
    if (shape=='O') return 'r';
    if (shape=='I') return 'b';
    if (shape=='J') return 'd';
    if (shape=='L') return 'o';
    if (shape=='S') return 'g';
    if (shape=='Z') return 'v';
    if (shape=='T') return 'y';
    return ' ';
}

void Grid::placeTetromino(tetromino &t, int tileSize) {
    int i = t.getPosition().y / tileSize;
    int j = t.getPosition().x / tileSize;
    char charColor  = TetrominoToChar(t);
    m_grid[i][j] = charColor;
    m_grid[i][j-1] = charColor;
    m_grid[i-1][j] = charColor;
    m_grid[i-1][j-1] = charColor;
}
