#include "saveManager.hpp"

SaveManager::SaveManager() : head(nullptr), saveFile("savegame.txt") {}

SaveManager::~SaveManager() {
    clearList();
}

void SaveManager::clearList() {
    TileNode* current = head;
    while (current != nullptr) {
        TileNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

void SaveManager::addTile(int x, int y) {
    TileNode* newNode = new TileNode(x, y);
    newNode->next = head;
    head = newNode;
}

TileNode* SaveManager::getTiles() {
    return head;
}

bool SaveManager::hasSaveGame() {
    ifstream file(saveFile);
    return file.good();
}

bool SaveManager::saveGame(const GameState& state, const string& username) {
    ofstream file(saveFile);
    if (!file.is_open()) return false;
    
    // Save Header
    file << username << endl;
    file << state.score << " " << state.lives << " " << state.level << endl;
    file << state.playerX << " " << state.playerY << endl;
    
    // Save Tiles (LinkedList)
    TileNode* current = head;
    while (current != nullptr) {
        file << current->x << " " << current->y << endl;
        current = current->next;
    }
    
    file.close();
    return true;
}

bool SaveManager::loadGame(GameState& state, string& username) {
    ifstream file(saveFile);
    if (!file.is_open()) return false;
    
    // Load Header
    file >> username;
    file >> state.score >> state.lives >> state.level;
    file >> state.playerX >> state.playerY;
    
    // Load Tiles
    clearList();
    int x, y;
    while (file >> x >> y) {
        addTile(x, y);
    }
    
    file.close();
    return true;
}
