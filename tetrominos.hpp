#pragma once
#include "raylib.h"
#include "grid.hpp"

#define tilesSize 30

class Vector {
public:
    int x;
    int y;
    Vector() : x(0), y(0) {}
    Vector(int xVal, int yVal) : x(xVal), y(yVal) {}

    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y);
    }
    Vector operator-(const Vector& other) const {
        return Vector(x - other.x, y - other.y);
    }
    Vector operator*(int scalar) const {
        return Vector(x * scalar, y * scalar);
    }
    Vector operator/(int scalar) const {
        return Vector(x / scalar, y / scalar);
    }
    bool operator==(const Vector& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }

    Vector& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector& operator-=(const Vector& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }   
};                   



// the position store the center of the tetromino, writen in pixels (not in grid cells),
// (tileSize,tileSize) is the top-left corner of the window because of lateral borders in the grid.
// The top left corner of the grid cell at position (i,j) have coordinates (x,y)=(j*tileSize,i*tileSize) in pixels.
class tetromino {
    char shape;
    Vector position;

public:
    tetromino() : shape('O'), position({0, 0}) {}
    tetromino(char s) : shape(s), position({0, 0}) {}
    tetromino(char s, Vector pos) : shape(s), position(pos * tilesSize) {}
    tetromino(const tetromino& other) : shape(other.shape), position(other.position) {}

    char getShape() const {
        return shape;
    }   

    Vector getPosition() const {
        return position;
    }
    void moveRight(int tileSize, Grid& grid) {
        int i = position.y / tileSize;
        int j = position.x / tileSize;
        if (grid.getCell(i, j + 1) == 0 && grid.getCell(i - 1, j + 1) == 0){
        position.x += tileSize;
        }
    }

    void moveLeft(int tileSize, Grid& grid) {
        int i = position.y / tileSize;
        int j = position.x / tileSize;
        if(grid.getCell(i, j - 2) == 0 && grid.getCell(i - 1, j - 2) == 0){
        position.x -= tileSize;
        }
    }
    
    // return true if the tetromino is grounded in its current position, and make it fall if not.
    bool fall(int tileSize, Grid& grid) {
        bool grounded = false;
        if(position.y % tileSize == 0){
            int i = position.y / tileSize;
            int j = position.x / tileSize;
            grounded = (grid.getCell(i + 1, j) != 0) || (grid.getCell(i + 1, j - 1) != 0);
            if(!grounded){
                position.y += tileSize;
            }
            return grounded;
        }
        position.y += tileSize;
        return grounded;
    }

    void draw(int tileSize) {
        int i = position.y / tileSize;
        int j = position.x / tileSize;
        Color color = charToColor(TetrominoToChar(*this));
        drawSquare(i,j,tileSize,color);
        drawSquare(i-1,j,tileSize,color);
        drawSquare(i,j-1,tileSize,color);
        drawSquare(i-1,j-1,tileSize,color);
    }
};