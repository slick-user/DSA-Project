#include "friendManager.hpp"

PlayerManager::PlayerManager() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        playerTable[i] = nullptr;  // FIXED: Initialize array elements to nullptr
    }
}

PlayerManager::~PlayerManager() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        PlayerNode* current = playerTable[i];
        while (current != nullptr) {
            PlayerNode* temp = current;
            current = current->next;

            // Clean up friend list for this player
            FriendNode* friendCurrent = temp->friends;
            while (friendCurrent != nullptr) {
                FriendNode* friendTemp = friendCurrent;
                friendCurrent = friendCurrent->next;
                delete friendTemp;
            }

            delete temp;
        }
    }
}

bool PlayerManager::playerExists(const string& username) {
    int index = hashFunction(username);
    PlayerNode* current = playerTable[index];

    while (current != nullptr) {
        if (current->username == username) {
            return true;
        }
        current = current->next;
    }
    return false;
}

PlayerNode* PlayerManager::getPlayer(const string& username) {
    int index = hashFunction(username);
    PlayerNode* current = playerTable[index];

    while (current != nullptr) {
        if (current->username == username) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void PlayerManager::addPlayer(const string& username) {
    if (playerExists(username)) return;

    int index = hashFunction(username);
    PlayerNode* newNode = new PlayerNode(username);
    newNode->next = playerTable[index];
    playerTable[index] = newNode;
}

// FriendManager
FriendManager::FriendManager(PlayerManager* pm) :
    playerManager(pm), currentUser(""), friendsFile("friends.txt"),
    requestsFile("friend_requests.txt"), pendingCount(0) {

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        friendsTable[i] = nullptr;
    }
    loadRequests();
}

FriendManager::~FriendManager() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        FriendNode* current = friendsTable[i];
        while (current != nullptr) {
            FriendNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

bool FriendManager::searchPlayer(const string& username) {
    if (playerManager == nullptr) return false;
    return playerManager->playerExists(username);
}



void FriendManager::setCurrentUser(const string& username) {
    currentUser = username;
    loadFriends();
    loadRequests();
}

void FriendManager::addFriend(const string& friendName) {
    if (isFriend(friendName)) return;

    // Add to Hash Table
    int index = hashFunction(friendName);
    FriendNode* newNode = new FriendNode(friendName);
    newNode->next = friendsTable[index];
    friendsTable[index] = newNode;

    // Add to File
    ofstream file(friendsFile, ios::app);
    if (file.is_open()) {
        file << currentUser << " " << friendName << endl;
        file.close();
    }
}

void FriendManager::removeFriend(const string& friendName) {
    // Remove from Hash Table
    int index = hashFunction(friendName);
    FriendNode* current = friendsTable[index];
    FriendNode* prev = nullptr;

    while (current != nullptr) {
        if (current->username == friendName) {
            if (prev == nullptr) {
                friendsTable[index] = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
            break;
        }
        prev = current;
        current = current->next;
    }

    // Remove from File (Rewrite)
    ifstream inFile(friendsFile);
    string lines[MAX_FRIENDS * 2]; // Buffer for file lines
    int lineCount = 0;
    string u, f;

    if (inFile.is_open()) {
        while (inFile >> u >> f && lineCount < MAX_FRIENDS * 2) {
            if (u == currentUser && f == friendName) {
                continue; // Skip this line
            }
            lines[lineCount] = u + " " + f;
            lineCount++;
        }
        inFile.close();
    }

    ofstream outFile(friendsFile);
    if (outFile.is_open()) {
        for (int i = 0; i < lineCount; i++) {
            outFile << lines[i] << endl;
        }
        outFile.close();
    }
}

bool FriendManager::isFriend(const string& friendName) {
    int index = hashFunction(friendName);
    FriendNode* current = friendsTable[index];
    while (current != nullptr) {
        if (current->username == friendName) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void FriendManager::getFriendsList(string* friendsArray, int& count) {
    count = 0;
    for (int i = 0; i < HASH_TABLE_SIZE && count < MAX_FRIENDS; i++) {
        FriendNode* current = friendsTable[i];
        while (current != nullptr && count < MAX_FRIENDS) {
            friendsArray[count] = current->username;
            count++;
            current = current->next;
        }
    }
}

int FriendManager::getFriendsCount() {
    int count = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        FriendNode* current = friendsTable[i];
        while (current != nullptr) {
            count++;
            current = current->next;
        }
    }
    return count;
}

// ==================== Friend Request System ====================

bool FriendManager::sendFriendRequest(const string& to) {
    // Safe PlayerManager check
    if (playerManager && !playerManager->playerExists(to)) {
        return false;  // Player not found
    }

    // Check if already friends
    if (isFriend(to)) return false;

    // Check if request already exists
    if (hasPendingRequest(currentUser, to)) return false;

    // Check if reverse request exists (they sent to us)
    if (hasPendingRequest(to, currentUser)) {
        // Auto-accept if they already sent us a request
        acceptFriendRequest(to);
        return true;
    }

    // Create new request if we have space
    if (pendingCount < MAX_REQUESTS) {
        pendingRequests[pendingCount] = FriendRequest(currentUser, to, time(0));
        pendingCount++;
        saveRequests();
        return true;
    }

    return false; // No space for new requests
}


bool FriendManager::acceptFriendRequest(const string& from) {
    int index = findRequest(from, currentUser);
    if (index == -1) return false;

    // Remove the request
    removeRequest(index);

    // Add both users as friends
    addFriend(from);

    // If PlayerManager exists, update the other player's friend list too
    if (playerManager) {
        PlayerNode* fromPlayer = playerManager->getPlayer(from);
        if (fromPlayer) {
            // Add current user to the other player's friend list
            FriendNode* newFriend = new FriendNode(currentUser);
            newFriend->next = fromPlayer->friends;
            fromPlayer->friends = newFriend;
        }
    }

    // Add reverse friendship to file
    ofstream file(friendsFile, ios::app);
    if (file.is_open()) {
        file << from << " " << currentUser << endl;
        file.close();
    }

    saveRequests();
    return true;
}


bool FriendManager::rejectFriendRequest(const string& from) {
    int index = findRequest(from, currentUser);
    if (index == -1) return false;

    removeRequest(index);
    saveRequests();
    return true;
}

void FriendManager::getPendingRequests(FriendRequest* requestsArray, int& count) {
    count = 0;
    for (int i = 0; i < pendingCount && count < MAX_REQUESTS; i++) {
        if (pendingRequests[i].to == currentUser) {
            requestsArray[count] = pendingRequests[i];
            count++;
        }
    }
}

void FriendManager::getSentRequests(FriendRequest* requestsArray, int& count) {
    count = 0;
    for (int i = 0; i < pendingCount && count < MAX_REQUESTS; i++) {
        if (pendingRequests[i].from == currentUser) {
            requestsArray[count] = pendingRequests[i];
            count++;
        }
    }
}

bool FriendManager::hasPendingRequest(const string& from, const string& to) {
    return findRequest(from, to) != -1;
}

int FriendManager::getPendingRequestsCount() {
    return pendingCount;
}

// ==================== Helper Methods ====================

void FriendManager::removeRequest(int index) {
    if (index < 0 || index >= pendingCount) return;

    // Shift all elements after index to the left
    for (int i = index; i < pendingCount - 1; i++) {
        pendingRequests[i] = pendingRequests[i + 1];
    }
    pendingCount--;
}

int FriendManager::findRequest(const string& from, const string& to) {
    for (int i = 0; i < pendingCount; i++) {
        if (pendingRequests[i].from == from && pendingRequests[i].to == to) {
            return i;
        }
    }
    return -1;
}

// ==================== File Operations ====================

void FriendManager::loadFriends() {
    // Clear current table
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        FriendNode* current = friendsTable[i];
        while (current != nullptr) {
            FriendNode* temp = current;
            current = current->next;
            delete temp;
        }
        friendsTable[i] = nullptr;
    }

    // Load from file
    ifstream file(friendsFile);
    string u, f;
    if (file.is_open()) {
        while (file >> u >> f) {
            if (u == currentUser) {
                // Add to Hash Table (without writing to file)
                int index = hashFunction(f);
                FriendNode* newNode = new FriendNode(f);
                newNode->next = friendsTable[index];
                friendsTable[index] = newNode;
            }
        }
        file.close();
    }
}

void FriendManager::loadRequests() {
    pendingCount = 0;
    ifstream file(requestsFile);
    if (file.is_open()) {
        string from, to;
        time_t ts;
        while (file >> from >> to >> ts && pendingCount < MAX_REQUESTS) {
            pendingRequests[pendingCount] = FriendRequest(from, to, ts);
            pendingCount++;
        }
        file.close();
    }
}

void FriendManager::saveRequests() {
    ofstream file(requestsFile);
    if (file.is_open()) {
        for (int i = 0; i < pendingCount; i++) {
            file << pendingRequests[i].from << " "
                << pendingRequests[i].to << " "
                << pendingRequests[i].timestamp << endl;
        }
        file.close();
    }
}