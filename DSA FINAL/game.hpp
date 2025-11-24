// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#pragma once
#include "enemy.hpp"
#include "utilities.hpp"
//#include "authManager.hpp"
#include "gameManager.hpp"

class Game {
public:
    Game(RenderWindow* window=nullptr);
    MenuOptions run();

    int getScore() const;

private:
    void processInput();
    void update(float dt);
    void render();

    RenderWindow* window;
    Texture tTile, tGameover, tEnemy;
    Sprite sTile, sGameover, sEnemy;

    Font font;
    Text scoreText, powerUpText, bonusText, freezeTimerText;

    int x, y, dx, dy;
    float timer, delay;
    bool GameRunning;

    Grid* grid;
    Enemy enemies[10];
    int enemyCount;

    int score;
    int lastPointsEarned;
    int bonusCounter;
    int tilesCapturedThisMove;
    int countCapturedTiles();
    int addScore(int tilesCaptured);

    int powerUpCount;
    int getPowerUpCount() const;
    void checkPowerUpAward();

    bool freezePowerUpActive;
    float freezePowerUpTimer;
    void activateFreezePowerUp();
    void updateFreezePowerUp(float dt);

    void drawUI();
    float displayTimer;
};