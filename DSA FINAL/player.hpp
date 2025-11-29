#pragma once
#include "utilities.hpp"

class GamePlayer {
public:
    GamePlayer(int startX, int startY, int playerNum, Grid* grid);
    
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getDX() const { return dx; }
    int getDY() const { return dy; }
    int getScore() const { return score; }
    bool isAlive() const { return alive; }
    bool isConstructing() const { return constructing; }
    bool hasPowerUp() const { return powerUpActive; }
    float getPowerUpTimer() const { return powerUpTimer; }
    int getPlayerNum() const { return playerNum; }
    
    // Setters
    void setScore(int s) { score = s; }
    void setAlive(bool a) { alive = a; }
    void setConstructing(bool c) { constructing = c; }
    
    // Movement
    void setDirection(int newDx, int newDy);
    void move(float delay, float dt);
    
    // Power-up
    int getPowerUpCount() const;
    void incrementPowerUpCount(int val);
    void activatePowerUp();
    void updatePowerUp(float dt);
    
    // Position check
    bool isAtPosition(int checkX, int checkY) const;
    
private:
    int x, y;           // Grid position
    int dx, dy;         // Direction
    int score;
    bool alive;
    bool constructing;  // True when moving on empty cells
    int playerNum;      // 1 or 2
    Grid* grid;
    
    // Timers
    float moveTimer;
    
    // Power-up
    int powerUpCount;
    bool powerUpActive;
    float powerUpTimer;
    const float POWERUP_DURATION = 3.0f;
};
