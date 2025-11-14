// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#pragma once

#include "utilities.hpp"

class Enemy {
private:
    int x, y, dx, dy;
    Grid* grid; 

public:

    // CONSTRUCTORS
    Enemy(int x = 300, int y = 300, Grid* grid=nullptr);

    // GETTERS
    int getX() const;
    int getY() const;

    // SETTERS
    void setPos(int x, int y);
    void setX(int x);
    void setY(int y);

    // REMAINING FUNCTIONS
    void move();
};