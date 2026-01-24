#pragma once
#include "raylib.h"
#include "constant.hpp"
#include <map>
#include <stdio.h>

class Grid; // forward declaration

class Position {
public:
    int i;
    int j;
    Position() : i(0), j(0) {}
    Position(int iVal, int jVal) : i(iVal), j(jVal) {}

    Position operator+(const Position& other) const {
        return Position(i + other.i, j + other.j);
    }
    Position operator-(const Position& other) const {
        return Position(i - other.i, j - other.j);
    }
    Position operator*(int scalar) const {
        return Position(i * scalar, j * scalar);
    }
    Position operator/(int scalar) const {
        return Position(i / scalar, j / scalar);
    }
    bool operator==(const Position& other) const {
        return i == other.i && j == other.j;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    Position& operator+=(const Position& other) {
        i += other.i;
        j += other.j;
        return *this;
    }

    Position& operator-=(const Position& other) {
        i -= other.i;
        j -= other.j;
        return *this;
    }
};                   

// the position of the tetromino is stored in (i,j) grid cell coordinates of the top left corner of its bounding box
class tetromino {
private:
    Position position;
    int rotationState = 0;
    int nbRotationStates;
    char shape;
    std::map<int, std::vector<Position>> cells;
    Position botPos;

public:
    // tetromino() : position({1, 1}), rotationState(0), shape(' ') {}
    // tetromino(Position pos, char shape) : position(pos), rotationState(0), shape(shape) {}
    tetromino(char aShape, Grid& grid): shape(aShape){
        switch(aShape) {
        case 'T': 
            nbRotationStates = 4;
            position = Position(0, NB_COLS / 2);
            cells[0] = {Position(0,1), Position(1,0), Position(1,1), Position(1,2)};
            cells[1] = {Position(0,1), Position(1,1), Position(1,2), Position(2,1)};
            cells[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,1)};
            cells[3] = {Position(0,1), Position(1,0), Position(1,1), Position(2,1)};
            botPos = bottomPosition(grid);
            break;
        case 'O':
            nbRotationStates = 1;
            position = Position(0, NB_COLS / 2);
            cells[0] = {Position(0,0), Position(0,1), Position(1,0), Position(1,1)};
            botPos = bottomPosition(grid);
            break;
        case 'I':
            nbRotationStates = 4;  
            position = Position(0, NB_COLS / 2 - 1);
            cells[0] = {Position(1,0), Position(1,1), Position(1,2), Position(1,3)};
            cells[1] = {Position(0,2), Position(1,2), Position(2,2), Position(3,2)};
            cells[2] = {Position(2,0), Position(2,1), Position(2,2), Position(2,3)};
            cells[3] = {Position(0,1), Position(1,1), Position(2,1), Position(3,1)};
            botPos = bottomPosition(grid);
            break;
        case 'J':
            nbRotationStates = 4;
            position = Position(0, NB_COLS / 2);
            cells[0] = {Position(0,0), Position(1,0), Position(1,1), Position(1,2)};
            cells[1] = {Position(0,1), Position(0,2), Position(1,1), Position(2,1)};
            cells[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,2)};
            cells[3] = {Position(0,1), Position(1,1), Position(2,0), Position(2,1)};
            botPos = bottomPosition(grid);
            break;
        case 'L':
            nbRotationStates = 4;
            position = Position(0, NB_COLS / 2);
            cells[0] = {Position(0,2), Position(1,0), Position(1,1), Position(1,2)};
            cells[1] = {Position(0,1), Position(1,1), Position(2,1), Position(2,2)};
            cells[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,0)};
            cells[3] = {Position(0,0), Position(0,1), Position(1,1), Position(2,1)};
            botPos = bottomPosition(grid);
            break;
        case 'S':
            nbRotationStates = 4;
            position = Position(0, NB_COLS / 2);
            cells[0] = {Position(0,1), Position(0,2), Position(1,0), Position(1,1)};
            cells[1] = {Position(0,1), Position(1,1), Position(1,2), Position(2,2)};
            cells[2] = {Position(1,1), Position(1,2), Position(2,0), Position(2,1)};
            cells[3] = {Position(0,0), Position(1,0), Position(1,1), Position(2,1)};
            botPos = bottomPosition(grid);
            break;
        case 'Z':
            nbRotationStates = 4;
            position = Position(0, NB_COLS / 2);
            cells[0] = {Position(0,0), Position(0,1), Position(1,1), Position(1,2)};
            cells[1] = {Position(0,2), Position(1,1), Position(1,2), Position(2,1)};
            cells[2] = {Position(1,0), Position(1,1), Position(2,1), Position(2,2)};
            cells[3] = {Position(0,1), Position(1,0), Position(1,1), Position(2,0)};
            botPos = bottomPosition(grid);
            break;
        default: // 'O' as default
            nbRotationStates = 1;
            position = Position(0, NB_COLS / 2);
            cells[0] = {Position(0,0), Position(0,1), Position(1,0), Position(1,1)};
            botPos = bottomPosition(grid);
        }
    }

    tetromino(const tetromino& other) : position(other.position), rotationState(other.rotationState), shape(other.shape) {
        cells = other.cells;
    }

    Position getPosition() const {
        return position;
    }

    char getShape() const {
        return shape;
    }

    void setPosition(Position newPosition) {
        position = newPosition;
    }

    const std::vector<Position>& getCells() const {
        return cells.at(rotationState);
    }

    bool canMoveTo(std::vector<Position>& newCells, Position newPosition, Grid& grid) {
        for (const auto& cell : newCells) {
            int i = newPosition.i + cell.i;
            int j = newPosition.j + cell.j;
            if (grid.getCell(i, j) != 0) {
                return false;
            }
        }
        return true;
    }

    void moveRight(Grid& grid) {
        Position newPosition = position + Position(0, 1);
        if (canMoveTo(cells[rotationState], newPosition, grid)) {
            position = newPosition;
            botPos = bottomPosition(grid);
        }
    }

    void moveLeft(Grid& grid) {
        Position newPosition = position + Position(0, -1);
        if (canMoveTo(cells[rotationState], newPosition, grid)) {
            position = newPosition;
            botPos = bottomPosition(grid);
        }
    }

    // return true if the tetromino is grounded in its current position, and make it fall if not.
    bool fall(Grid& grid) {
        Position newPosition = position + Position(1, 0);
        if (canMoveTo(cells[rotationState], newPosition, grid)) {
            position = newPosition;
            return false;
        }
        return true;
    }

    void rotateClockwise(Grid& grid) {
        int newRotationState = (rotationState + 1) % nbRotationStates;
        std::vector<Position> newCells = cells[newRotationState];
        if (canMoveTo(newCells, position, grid)) {
            rotationState = newRotationState;
            botPos = bottomPosition(grid);
        }
    }

    void rotateCounterClockwise(Grid& grid) {
        int newRotationState = (rotationState + nbRotationStates - 1) % nbRotationStates;
        std::vector<Position> newCells = cells[newRotationState];
        if (canMoveTo(newCells, position, grid)) {
            rotationState = newRotationState;
            botPos = bottomPosition(grid);
        }
    }

    void draw(bool transparent = false); // Forward declaration, implementation after includes

    Position bottomPosition(Grid& grid){
        Position testPosition = position;
        while (canMoveTo(cells[rotationState], testPosition + Position(1,0), grid)) {
            testPosition += Position(1,0);
        }
        return testPosition;
    }

    void fastFall(){
        position = botPos;
    }
};

// Include after class definition to avoid circular includes
#include "draw.hpp"
#include "grid.hpp"

// Implementation of draw() after all includes
inline void tetromino::draw(bool transparent) {
    std::vector<Position> currentCells = cells[rotationState];
    if(transparent){
        for (const auto& cell : currentCells) {
        int i = botPos.i + cell.i;
        int j = botPos.j + cell.j;
            if (i > 0) { // do not draw above the grid
                drawSquareInGrid(i, j, charToColor(shapeToChar(shape)), true);
            }
        }
    }
    for (const auto& cell : currentCells) {
        int i = position.i + cell.i;
        int j = position.j + cell.j;
            if (i > 0) { // do not draw above the grid
                drawSquareInGrid(i, j, charToColor(shapeToChar(shape)), false);
            }
    }
}
