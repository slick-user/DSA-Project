// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#pragma once
#include "enemy.hpp"
#include "utilities.hpp"
#include "leaderboard.hpp"
#include "player.hpp"
#include "gameManager.hpp"
#include "saveManager.hpp"

class Game {
public:
    Game(RenderWindow* window, const string& username = "", int playerID = 0, 
         GameLevel level = EASY, GameMode mode = SINGLE_PLAYER_MODE,
         const string& username2 = "", int playerID2 = 0, Color bgColor = Color::Black);
    ~Game();
    MenuOptions run();

    int getScore() const;
    int getPlayer1Score() const;
    int getPlayer2Score() const;
    
    // Save/Load
    void saveGame();
    void loadGame();

private:
    void processInput();
    void update(float dt);
    void render();
    void checkCollisions();
    void checkPlayerCollisions();

    RenderWindow* window;
    // Save Manager
    SaveManager saveManager;
    GameManager* gameManager;
    Color bgColor;
    Texture tTile, tGameover, tEnemy, tPowerUp;
    Sprite sTile, sGameover, sEnemy, sPowerUp;

    // Single player mode
    int x, y, dx, dy;
    
    // Multiplayer mode
    GamePlayer* player1;
    GamePlayer* player2;
    bool isMultiplayer;
    
    float timer, delay;
    bool GameRunning;

    Grid* grid;
    Enemy* enemies;
    int enemyCount;
    bool enemiesFrozen;
    float enemyFreezeTimer;

    int score;
    string currentUsername;
    int currentPlayerID;
    string player2Username;
    int player2PlayerID;
    
    Leaderboard* leaderboard;
    int addScore(int tilesCaptured);
    int countCapturedTiles();
    void calculateScore();
    void calculateMultiplayerScores();
    void checkPowerUpAward(GamePlayer* p);

    void capturePlayerArea(int playerNum);
    bool hasEnclosedArea(int playerNum);
    void updateMultiplayerPlayer(GamePlayer* player, int playerNum, float dt);
    void clearPlayerTrails(int playerNum);
    
    int lastPointsEarned;
    int bonusCounter;
    int tilesCapturedThisMove;
    int countNewlyCapturedTiles();

    // don't know if we need this
    void drawMultiplayerUI(const Font& font);
    void drawSinglePlayerUI(const Font& font);
    void drawGameOverScreen(const Font& font);

    void updatePlayerScore(GamePlayer* player, int tilesCaptured);

    GameLevel currentLevel;
    GameMode currentMode;
    int targetPercentage;
};