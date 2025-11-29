#pragma once
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include "leaderboard.hpp"

using namespace std;

struct QueuePlayer {
    string username;
    int score;
    int rank;
    
    QueuePlayer(string u = "", int s = 0, int r = 0) : username(u), score(s), rank(r) {}
    
    // Comparison for Priority Queue (Max-Heap based on score)
    bool operator<(const QueuePlayer& other) const {
        return score < other.score;
    }
};

struct GameRoom {
    QueuePlayer player1;
    QueuePlayer player2;
    int roomID;
    bool active;
    
    GameRoom(QueuePlayer p1 = QueuePlayer(), QueuePlayer p2 = QueuePlayer(), int id = 0) 
        : player1(p1), player2(p2), roomID(id), active(true) {}
};

class MatchmakingQueue {
private:
    vector<QueuePlayer> heap;
    queue<GameRoom> waitingRooms;
    int nextRoomID;
    
    void heapifyUp(int index);
    void heapifyDown(int index);
    
public:
    MatchmakingQueue();
    
    // Player management
    void addPlayer(const string& username, int score, int rank);
    void addPlayerFromLeaderboard(const string& username, Leaderboard& lb);
    QueuePlayer extractMax(); // Get best match (highest score)
    QueuePlayer peekMax();
    bool isEmpty();
    int getSize();
    
    // Auto-pairing system
    bool canMatch(); // Check if >= 2 players
    pair<QueuePlayer, QueuePlayer> matchTopTwo();
    void autoMatch(); // Continuously match while possible
    
    // Game room management
    int createRoom(QueuePlayer p1, QueuePlayer p2);
    GameRoom getNextRoom();
    bool hasWaitingRooms();
    int getRoomCount();
    
    // Utility
    QueuePlayer findMatch(int targetScore, int tolerance);
    vector<QueuePlayer> getAllPlayers();
};
