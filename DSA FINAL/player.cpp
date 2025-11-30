// Azlan Ali Khan 24I-2110, Sameer Hussain 24I-2042, DSA FINAL PROJECT
#include "player.hpp"

GamePlayer::GamePlayer(int startX, int startY, int playerNum, Grid* grid)
                     : x(startX), y(startY), dx(0), dy(0), score(0), alive(true),
                       constructing(false), playerNum(playerNum), grid(grid),
                       moveTimer(0.0f), powerUpCount(0), powerUpActive(false), powerUpTimer(0.0f) {}

void GamePlayer::setDirection(int newDx, int newDy) {
    dx = newDx;
    dy = newDy;
}

void GamePlayer::move(float delay, float dt) {
    if (!alive) return;

    moveTimer += dt;

    if (moveTimer >= delay) {
        x += dx;
        y += dy;

        // Boundary check
        if (x < 0) x = 0;
        if (x > grid->getCols() - 1) x = grid->getCols() - 1;
        if (y < 0) y = 0;
        if (y > grid->getRows() - 1) y = grid->getRows() - 1;

        moveTimer = 0;
    }
}


int GamePlayer::getPowerUpCount() const { return powerUpCount; }

void GamePlayer::incrementPowerUpCount(int val) {
    powerUpCount += val;
    if (powerUpCount < 0) powerUpCount = 0;
}

void GamePlayer::activatePowerUp() {
    powerUpActive = true;
    powerUpTimer = POWERUP_DURATION;
}

void GamePlayer::updatePowerUp(float dt) {
    if (powerUpActive) {
        powerUpTimer -= dt;
        if (powerUpTimer <= 0) {
            powerUpActive = false;
            powerUpTimer = 0;
        }
    }
}

bool GamePlayer::isAtPosition(int checkX, int checkY) const {
    return (x == checkX && y == checkY);
}
