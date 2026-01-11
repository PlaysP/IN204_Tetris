#include "grid.hpp"
#include "tetrominos.hpp"
#include "constant.hpp"


void Grid::placeTetromino(tetromino &t) {
    std::vector<Position> currentCells = t.getCells();
    Position pos = t.getPosition();
    char c = shapeToChar(t.getShape());
    for (const auto& cell : currentCells) {
        int i = pos.i + cell.i;
        int j = pos.j + cell.j;
        m_grid[i][j] = c;
    }
}
