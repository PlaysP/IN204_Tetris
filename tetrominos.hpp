#pragma once
#include "raylib.h"
#include "grid.hpp"
#include "constant.hpp"
#include <map>
#include <stdio.h>

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
    char shape;

protected:
    std::map<int, std::vector<Position>> cells;

    // tetromino() : position({1, 1}), rotationState(0), shape(' ') {}
    tetromino(Position pos, char shape) : position(pos), rotationState(0), shape(shape) {}
    tetromino(const tetromino& other) : position(other.position), rotationState(other.rotationState), shape(other.shape) {
        cells = other.cells;
    }

public:
    Position getPosition() const {
        return position;
    }

    char getShape() const {
        return shape;
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
        }
    }

    void moveLeft(Grid& grid) {
        Position newPosition = position + Position(0, -1);
        if (canMoveTo(cells[rotationState], newPosition, grid)) {
            position = newPosition;
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
        int newRotationState = (rotationState + 1) % 4;
        std::vector<Position> newCells = cells[newRotationState];
        if (canMoveTo(newCells, position, grid)) {
            rotationState = newRotationState;
        }
    }

    void rotateCounterClockwise(Grid& grid) {
        int newRotationState = (rotationState + 3) % 4;
        std::vector<Position> newCells = cells[newRotationState];
        if (canMoveTo(newCells, position, grid)) {
            rotationState = newRotationState;
        }
    }

    void draw() {
        std::vector<Position> currentCells = cells[rotationState];
        for (const auto& cell : currentCells) {
            int i = position.i + cell.i;
            int j = position.j + cell.j;
            drawSquare(i, j, TILE_SIZE, charToColor(shapeToChar(shape)));
        }
    }
};

class tetromino_T : public tetromino {

    public:
    tetromino_T(): tetromino({0, NB_COLS/2}, 'T') {
        cells[0] = {Position(0,1), Position(1,0), Position(1,1), Position(1,2)};
        cells[1] = {Position(0,1), Position(1,1), Position(1,2), Position(2,1)};
        cells[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,1)};
        cells[3] = {Position(0,1), Position(1,0), Position(1,1), Position(2,1)};
    }
};

class tetromino_O : public tetromino {
    public:
    tetromino_O(): tetromino({0, NB_COLS/2}, 'O') {
        cells[0] = {Position(0,0), Position(0,1), Position(1,0), Position(1,1)};
    }
};

class tetromino_I : public tetromino {
    public:
    tetromino_I(): tetromino({0, NB_COLS/2-1}, 'I') {
        cells[0] = {Position(1,0), Position(1,1), Position(1,2), Position(1,3)};
        cells[1] = {Position(0,2), Position(1,2), Position(2,2), Position(3,2)};
        cells[2] = {Position(2,0), Position(2,1), Position(2,2), Position(2,3)};
        cells[3] = {Position(0,1), Position(1,1), Position(2,1), Position(3,1)};
    }
};

class tetromino_J : public tetromino {
    public:
    tetromino_J(): tetromino({0, NB_COLS/2}, 'J') {
        cells[0] = {Position(0,0), Position(1,0), Position(1,1), Position(1,2)};
        cells[1] = {Position(0,1), Position(0,2), Position(1,1), Position(2,1)};
        cells[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,2)};
        cells[3] = {Position(0,1), Position(1,1), Position(2,0), Position(2,1)};
    }
};

class tetromino_L : public tetromino {
    public:
    tetromino_L(): tetromino({0, NB_COLS/2}, 'L') {
        cells[0] = {Position(0,2), Position(1,0), Position(1,1), Position(1,2)};
        cells[1] = {Position(0,1), Position(1,1), Position(2,1), Position(2,2)};
        cells[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,0)};
        cells[3] = {Position(0,0), Position(0,1), Position(1,1), Position(2,1)};
    }
};

class tetromino_S : public tetromino {
    public:
    tetromino_S(): tetromino({0, NB_COLS/2}, 'S') {
        cells[0] = {Position(0,1), Position(0,2), Position(1,0), Position(1,1)};
        cells[1] = {Position(0,1), Position(1,1), Position(1,2), Position(2,2)};
        cells[2] = {Position(1,1), Position(1,2), Position(2,0), Position(2,1)};
        cells[3] = {Position(0,0), Position(1,0), Position(1,1), Position(2,1)};
    }
};

class tetromino_Z : public tetromino {
    public:
    tetromino_Z(): tetromino({0, NB_COLS/2}, 'Z') {
        cells[0] = {Position(0,0), Position(0,1), Position(1,1), Position(1,2)};
        cells[1] = {Position(0,2), Position(1,1), Position(1,2), Position(2,1)};
        cells[2] = {Position(1,0), Position(1,1), Position(2,1), Position(2,2)};
        cells[3] = {Position(0,1), Position(1,0), Position(1,1), Position(2,0)};
    }
};