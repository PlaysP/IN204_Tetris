#pragma once

#include <vector>
#include <iostream>
#include "raylib.h"

Color charToColor(char c) {
    if (c=='r') return RED;
    if (c=='y') return GOLD;
    if (c=='b') return BLUE;
    if (c=='g') return LIME;
    if (c=='o') return ORANGE;
    if (c=='v') return VIOLET;
    return BEIGE;
}

// Grid size: 20 x 10
// i - component : screenHeight, nb of rows [0, 19]
// j - component: screenWidth, nb of colomns [0, 9]
class Grid {
private:
    int nbRow = 20;
    int nbCol = 10;

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

        test :
        for (int j = 0; j<jMax; j++) m_grid[6][j]='r';
        m_grid[5][4]='y';
        m_grid[7][6]='y';
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

    void removeFilledRows() {
        for (int i = 1; i < iMax-1; i++) {
            if (isFilled(m_grid[i])) {
                std::vector<std::vector<char>>::const_iterator begin = m_grid.begin();
                m_grid.erase(begin + i);
                std::vector<char> newRow(jMax,0);
                newRow[0] = 1;
                newRow[jMax-1] = 1;
                m_grid.insert(begin+1,newRow);
            }
        }
    }

    void draw(int tileSize) {
        // Background grid pattern
        ClearBackground(BLACK);
        for (int i=0; i<iMax-1; i++) {
            for (int j=0; j<jMax-1; j++) {
                DrawRectangleLines(j*tileSize, i*tileSize, tileSize, tileSize, DARKGRAY);
            }
        }

        for (int i = 1; i < iMax-1; i++) {
            for (int j = 1; j < jMax-1; j++) {
                if (m_grid[i][j] != 0) {
                    Color color = charToColor(m_grid[i][j]);
                    DrawRectangleGradientH((j-1)*tileSize, (i-1)*tileSize, tileSize, tileSize, color, ColorBrightness(color, -0.3f));
                }
            }
        }
    }

    char getCell(int i, int j) const {
        return m_grid[i][j];
    }
};