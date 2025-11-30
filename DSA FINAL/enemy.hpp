// Azlan Ali Khan 24I-2110, Sameer Hussain 24I-2042, DSA FINAL PROJECT

#pragma once
#include "utilities.hpp"

class Enemy {
public:
    Enemy(int x = 0, int y = 0, Grid* grid = nullptr);

    int getX() const;
    int getY() const;

    void setPos(int x, int y);
    void setX(int x);
    void setY(int y);

    void move();

private:
    int x, y;
    int dx, dy;
    Grid* grid;
};