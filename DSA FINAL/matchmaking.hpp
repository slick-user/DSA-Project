// Azlan Ali Khan 24I-2110, Sameer Hussain 24I-2042, DSA FINAL PROJECT
#pragma once
#include <string>
#include <iostream>
#include "leaderboard.hpp"

using namespace std;

struct QueuePlayer {
    string username;
    int score;
    int rank;
    int id;

    QueuePlayer(string u = "", int s = 0, int r = 0, int id=0) : 
                username(u), score(s), rank(r), id(id) {}

    // Comparison for Priority Queue (Max-Heap based on score)
    bool operator<(const QueuePlayer& other) const {
        return score < other.score;
    }
};

// Custom Pair replacement
struct PlayerPair {
    QueuePlayer first;
    QueuePlayer second;

    PlayerPair(QueuePlayer f = QueuePlayer(), QueuePlayer s = QueuePlayer())
        : first(f), second(s) {}

    bool isValid() const {
        return !first.username.empty() && !second.username.empty();
    }
};

struct GameRoom {
    QueuePlayer player1;
    QueuePlayer player2;
    int roomID;
    bool active;
    int player1Score;
    int player2Score;
    bool gameStarted;
    bool gameCompleted;

    GameRoom(QueuePlayer p1 = QueuePlayer(), QueuePlayer p2 = QueuePlayer(), int id = 0)
        : player1(p1), player2(p2), roomID(id), active(true),
        player1Score(0), player2Score(0), gameStarted(false), gameCompleted(false) {}

    void startGame() {
        gameStarted = true;
        cout << "Game started in Room " << roomID << ": "
            << player1.username << " vs " << player2.username << endl;
    }

    void endGame(int p1Score, int p2Score) {
        player1Score = p1Score;
        player2Score = p2Score;
        gameCompleted = true;
        active = false;
        cout << "Game completed in Room " << roomID << ": "
            << player1.username << " (" << p1Score << ") vs "
            << player2.username << " (" << p2Score << ")" << endl;
    }

    string getWinner() const {
        if (!gameCompleted) return "Game not completed";
        if (player1Score > player2Score) return player1.username;
        if (player2Score > player1Score) return player2.username;
        return "Tie";
    }

    bool isPlayerInRoom(const string& username) const {
        return player1.username == username || player2.username == username;
    }
};

// Custom Vector implementation
template<typename T>
class Vector {
private:
    T* data;
    int capacity;
    int size;

    void resize(int newCapacity) {
        T* newData = new T[newCapacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Vector() : data(new T[10]), capacity(10), size(0) {}

    ~Vector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (size >= capacity) {
            resize(capacity * 2);
        }
        data[size++] = value;
    }

    void pop_back() {
        if (size > 0) {
            size--;
        }
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    T& back() {
        return data[size - 1];
    }

    const T& back() const {
        return data[size - 1];
    }

    bool empty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }

    void clear() {
        size = 0;
    }
};

template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* front;
    Node* back;
    int queueSize;

public:
    Queue() : front(nullptr), back(nullptr), queueSize(0) {}

    ~Queue() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& value) {
        Node* newNode = new Node(value);
        if (back == nullptr) {
            front = back = newNode;
        }
        else {
            back->next = newNode;
            back = newNode;
        }
        queueSize++;
    }

    void pop() {
        if (empty()) return;

        Node* temp = front;
        front = front->next;
        if (front == nullptr) {
            back = nullptr;
        }
        delete temp;
        queueSize--;
    }

    T& getFront() {
        return front->data;
    }

    const T& getFront() const {
        return front->data;
    }

    bool empty() const {
        return front == nullptr;
    }

    int size() const {
        return queueSize;
    }
};

class MatchmakingQueue {
private:
    Vector<QueuePlayer> heap;
    Queue<GameRoom> waitingRooms;
    Vector<GameRoom> activeRooms;
    int nextRoomID;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    MatchmakingQueue();

    // Player management
    void addPlayer(const string& username, int score, int rank, int playerID);
    void addPlayerFromLeaderboard(const string& username, Leaderboard& lb, int playerID);
    QueuePlayer extractMax(); // Get best match (highest score)
    QueuePlayer peekMax();
    bool isEmpty();
    int getSize();

    // Auto-pairing system
    bool canMatch(); // Check if >= 2 players
    PlayerPair matchTopTwo(); // Custom pair instead of std::pair
    void autoMatch(); // Continuously match while possible

    // Game room management
    int createRoom(QueuePlayer p1, QueuePlayer p2);
    GameRoom getNextRoom();
    bool hasWaitingRooms();
    int getRoomCount();

    // Active room management
    void startGame(int roomID);
    void endGame(int roomID, int player1Score, int player2Score);
    GameRoom* findRoomByPlayer(const string& username);
    Vector<GameRoom> getActiveRooms();
    void cleanupCompletedRooms();

    // Utility
    QueuePlayer findMatch(int targetScore, int tolerance);
    Vector<QueuePlayer> getAllPlayers();
};