// leaderboard.hpp
#pragma once
#include <string>
using namespace std;

const int LEADERBOARD_SIZE = 10;

struct LeaderboardEntry {
    string username;
    int score;
    int playerID;
    
    LeaderboardEntry() : username(""), score(0), playerID(0) {}
    LeaderboardEntry(const string& name, int s, int id) : username(name), score(s), playerID(id) {}
};

class Leaderboard {
private:
    LeaderboardEntry heap[LEADERBOARD_SIZE];
    int size;
    
    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }
    
    void swapEntries(int i, int j);
    void heapifyUp(int index);
    void heapifyDown(int index);
    
public:
    Leaderboard();
    
    void addOrUpdate(const string& username, int score, int playerID);
    bool shouldAdd(int score) const;
    int getMinScore() const;
    int getSize() const { return size; }
    
    void getSortedEntries(LeaderboardEntry sorted[]) const;
    bool loadFromFile();
    bool saveToFile();
    void display() const;
};