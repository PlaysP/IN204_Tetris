#include "raylib.h"
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
    void moveRight(int tileSize,Grid &grid) {
        int i = position.y / tileSize;
        int j = position.x / tileSize;
        if(grid.getCell(i, j + 2) == 0 && grid.getCell(i - 1, j + 2) == 0){
        position.x += tileSize;
        }
    }

    void moveLeft(int tileSize,Grid &grid) {
        int i = position.y / tileSize;
        int j = position.x / tileSize;
        if(grid.getCell(i, j - 1) == 0 && grid.getCell(i - 1, j - 1) == 0){
        position.x -= tileSize;
        }
    }
    
    bool fall(int tileSize,Grid &grid) {
        bool grounded = false;
        if(position.y % tileSize == 0){
            int i = position.y / tileSize;
            int j = position.x / tileSize;
            grounded = (grid.getCell(i + 2, j) != 0) || (grid.getCell(i + 2, j - 1) != 0);
            if(!grounded){
                position.y += tileSize;
            }
            return grounded;
        }
        position.y += tilesSize;
        return grounded;
    }

    void draw(int tileSize) {
        int x = position.x;
        int y = position.y;
        DrawRectangle(x, y, tileSize, tileSize,RED);
        DrawRectangleLines(x, y, tileSize, tileSize, BLACK);

        DrawRectangle(x - tileSize, y, tileSize, tileSize,RED);
        DrawRectangleLines(x - tileSize, y, tileSize, tileSize, BLACK);

        DrawRectangle(x - tileSize, y - tileSize, tileSize, tileSize,RED);
        DrawRectangleLines(x - tileSize, y - tileSize, tileSize, tileSize, BLACK);     

        DrawRectangle(x, y - tileSize, tileSize, tileSize,RED);
        DrawRectangleLines(x, y - tileSize, tileSize, tileSize, BLACK);
    }
};