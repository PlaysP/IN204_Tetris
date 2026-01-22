#pragma once

#include <vector>
#include <iostream>
#include "raylib.h"
#include "constant.hpp"

class tetromino; // forward declaration to avoid circular include

// Grid size: 20 x 10
// i - component : screenHeight, playable rows [1, 20]
// j - component: screenWidth, playable colomns [1, 10]
class Grid {
private:
    int nbRow = NB_ROWS;
    int nbCol = NB_COLS;

    // jMax takes into account lateral borders and iMax inferior border and the top row where tetrominos spawn
    int iMax = nbRow+2;
    int jMax = nbCol+2;

    std::vector<std::vector<char>> m_grid;

    // return true if the row is filled
    bool isFilled(std::vector<char>& row) {
        for (int j=1; j<jMax-1; j++) {
            if (row[j]==0) return false;
        }
        return true;
    }
public:
    Grid(): m_grid() {
        // 0 = free
        m_grid.resize(iMax, std::vector<char>(jMax,0));
        // 1 = borders
        for (int j = 0; j < jMax; j++) m_grid[iMax-1][j] = 1;
        for (int i = 0; i < iMax; i++) {
            m_grid[i][0] = 1;
            m_grid[i][jMax-1] = 1;
        }
    }
    Grid(const Grid& aGrid): m_grid(aGrid.m_grid) {};

    void print() {
        for (int i=0; i<iMax; i++) {
            for (int j=0; j<jMax; j++) {
                if (m_grid[i][j]==0) std::cout << ". ";
                else if (m_grid[i][j]==1) std::cout << "# ";
                else std::cout << m_grid[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void reset() {
        for (int i=0; i<iMax-1; i++) {
            for (int j=1; j<jMax-1; j++) m_grid[i][j]=0;
        }
        for (int j = 0; j < jMax; j++) m_grid[iMax-1][j] = 1;
        for (int i = 0; i < iMax; i++) {
            m_grid[i][0] = 1;
            m_grid[i][jMax-1] = 1;
        }
    }

    int removeFilledRows() {
        int rowsRemoved = 0;
        for (int i = 1; i < iMax-1; i++) {
            if (isFilled(m_grid[i])) {
                rowsRemoved++;
                std::vector<std::vector<char>>::const_iterator begin = m_grid.begin();
                m_grid.erase(begin + i);
                std::vector<char> newRow(jMax,0);
                newRow[0] = 1;
                newRow[jMax-1] = 1;
                m_grid.insert(begin+1,newRow);
            }
        }
        return rowsRemoved;
    }

    void draw(); // Forward declaration, implementation after draw.hpp is included

    char getCell(int i, int j) const {
        return m_grid[i][j];
    }

    void placeTetromino(tetromino &t);

    bool GameOver() {
        for (int j = 1; j < jMax-1; j++) {
            if (m_grid[1][j] != 0) {
                return true;
            }
        }
        return false;
    }
};

// Include after class definition to avoid circular includes
#include "draw.hpp"

// Implementation of draw() after draw.hpp is included
inline void Grid::draw() {
    // Background grid pattern

    DrawRectangle(GRID_X_OFFSET, GRID_Y_OFFSET, GRID_WIDTH, GRID_HEIGHT, BLACK);

    for (int i=0; i<nbRow; i++) {
        for (int j=0; j<nbCol; j++) {
            DrawRectangleLines(GRID_X_OFFSET + j*TILE_SIZE, GRID_Y_OFFSET + i*TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
        }
    }

    for (int i = 1; i < iMax-1; i++) {
        for (int j = 1; j < jMax-1; j++) {
            if (m_grid[i][j] != 0) {
                Color color = charToColor(m_grid[i][j]);
                drawSquareInGrid(i, j, color);
            }
        }
    }
}