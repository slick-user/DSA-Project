// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#pragma once
#include "enemy.hpp"
#include "utilities.hpp"

class Game {
public:
    Game(RenderWindow* window=nullptr);
    MenuOptions run();

private:
    void processInput();
    void update(float dt);
    void render();

    RenderWindow* window;
    Texture tTile, tGameover, tEnemy;
    Sprite sTile, sGameover, sEnemy;

    int x, y, dx, dy;
    float timer, delay;
    bool GameRunning;

    Grid* grid;
    Enemy enemies[10];
    int enemyCount;
};