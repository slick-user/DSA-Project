#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

const int HASH_TABLE_SIZE = 50;
const int MAX_FRIENDS = 100;
const int MAX_REQUESTS = 100;

struct FriendNode;

struct PlayerNode {
    string username;
    FriendNode* friends;  // Linked list of friends
    PlayerNode* next;     // For hash table collision

    PlayerNode(string u) : username(u), friends(nullptr), next(nullptr) {}
};

struct FriendNode {
    string username;
    FriendNode* next;

    FriendNode(string u) : username(u), next(nullptr) {}
};

struct FriendRequest {
    string from;
    string to;
    time_t timestamp;

    FriendRequest(string f = "", string t = "", time_t ts = 0)
        : from(f), to(t), timestamp(ts) {}
};

class PlayerManager {
private:
    PlayerNode* playerTable[HASH_TABLE_SIZE];  // Hash table for ALL players

    int hashFunction(const string& key) {
        int sum = 0;
        for (char c : key) {
            sum += c;
        }
        return sum % HASH_TABLE_SIZE;
    }

public:
    PlayerManager();
    ~PlayerManager();

    bool playerExists(const string& username);
    void addPlayer(const string& username);
    PlayerNode* getPlayer(const string& username);
    // REMOVED duplicate hashFunction declaration
};

class FriendManager {
private:
    PlayerManager* playerManager;
    FriendNode* friendsTable[HASH_TABLE_SIZE];
    string currentUser;  // REMOVED duplicate declaration
    string friendsFile;
    string requestsFile;

    FriendRequest pendingRequests[MAX_REQUESTS];
    int pendingCount;

    int hashFunction(const string& key) {
        int sum = 0;
        for (char c : key) {
            sum += c;
        }
        return sum % HASH_TABLE_SIZE;
    }

public:
    FriendManager(PlayerManager* pm = nullptr);
    ~FriendManager();

    // Search functionality using hash table
    bool searchPlayer(const string& username);

    void setCurrentUser(const string& username);

    // Direct friend management
    void addFriend(const string& friendName);
    void removeFriend(const string& friendName);
    bool isFriend(const string& friendName);
    void getFriendsList(string* friendsArray, int& count);
    int getFriendsCount();

    // Friend request system
    bool sendFriendRequest(const string& to);
    bool acceptFriendRequest(const string& from);
    bool rejectFriendRequest(const string& from);
    void getPendingRequests(FriendRequest* requestsArray, int& count);
    void getSentRequests(FriendRequest* requestsArray, int& count);
    bool hasPendingRequest(const string& from, const string& to);
    int getPendingRequestsCount();

    // File operations
    void loadFriends();
    void saveFriends();
    void loadRequests();
    void saveRequests();

private:
    void removeRequest(int index);
    int findRequest(const string& from, const string& to);
};