// Azlan Ali Khan 24I-2110, Sameer Hussain 24I-2042, DSA FINAL PROJECT

#include "enemy.hpp"

Enemy::Enemy(int x, int y, Grid* grid) {
	this->x = x;
    this->y = y;
	
    dx = 4 - rand() % 8;
	dy = 4 - rand() % 8;

    this->grid = grid;
}

// GETTERS
int Enemy::getX() const { return x; }
int Enemy::getY() const { return y; }

// SETTERS
void Enemy::setPos(int x, int y) { this->x = x; this->y = y; }
void Enemy::setX(int x) { this->x = x; }
void Enemy::setY(int y) { this->y = y; }

void Enemy::move() {
    int nextX = x + dx;
    int nextY = y + dy;

    // Check horizontal collision
    if (grid->getCell(y / ts, nextX / ts) == 1)
        dx = -dx;
    else
        x = nextX;

    // Check vertical collision
    if (grid->getCell(nextY / ts, x / ts) == 1)
        dy = -dy;
    else
        y = nextY;
}
