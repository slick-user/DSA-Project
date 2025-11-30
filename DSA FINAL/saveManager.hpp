// Azlan Ali Khan 24I-2110, Sameer Hussain 24I-2042, DSA FINAL PROJECT
#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "utilities.hpp"

using namespace std;

// LinkedList Node for storing filled tiles
struct TileNode {
    int x, y;
    TileNode* next;
    
    TileNode(int col, int row) : x(col), y(row), next(nullptr) {}
};

struct GameState {
    int score;
    int lives;
    int level;
    int playerX, playerY;
    // We will store tiles in the LinkedList
};

class SaveManager {
private:
    TileNode* head;
    string saveFile;
    
    void clearList();
    
public:
    SaveManager();
    ~SaveManager();
    
    void addTile(int x, int y);
    TileNode* getTiles();
    
    bool saveGame(const GameState& state, const string& username);
    bool loadGame(GameState& state, string& username);
    bool hasSaveGame();
};
