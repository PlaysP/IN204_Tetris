#include "raylib.h"

class tetromino {
    char shape;
    Vector2 position;


    tetromino() : shape('O'), position({0, 0}) {}
    tetromino(char s) : shape(s), position({0, 0}) {}
    tetromino(char s, Vector2 pos) : shape(s), position(pos) {}
    tetromino(const tetromino& other) : shape(other.shape), position(other.position) {}

    public:
    char getShape() const {
        return shape;
    }   

    Vector2 getPosition() const {
        return position;
    }
};