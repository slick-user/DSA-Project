// leaderboard.cpp
#include "leaderboard.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

Leaderboard::Leaderboard() : size(0) {
    loadFromFile();
}

void Leaderboard::swapEntries(int i, int j) {
    LeaderboardEntry temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void Leaderboard::heapifyUp(int index) {
    while (index > 0 && heap[parent(index)].score > heap[index].score) {
        swapEntries(index, parent(index));
        index = parent(index);
    }
}

void Leaderboard::heapifyDown(int index) {
    int smallest = index;
    int left = leftChild(index);
    int right = rightChild(index);
    
    if (left < size && heap[left].score < heap[smallest].score) {
        smallest = left;
    }
    
    if (right < size && heap[right].score < heap[smallest].score) {
        smallest = right;
    }
    
    if (smallest != index) {
        swapEntries(index, smallest);
        heapifyDown(smallest);
    }
}

bool Leaderboard::shouldAdd(int score) const {
    if (score <= 0) return false;
    //return size < LEADERBOARD_SIZE || score > getMinScore();
    return score > getMinScore();
}

void Leaderboard::addOrUpdate(const string& username, int score, int playerID) {
    cout << "DEBUG: Leaderboard addOrUpdate called - " << username << ": " << score << endl;
    
    // Check if player already exists in leaderboard
    for (int i = 0; i < size; i++) {
        if (heap[i].username == username) {
            cout << "DEBUG: Updating existing player: " << username << endl;
            if (score > heap[i].score) {
                heap[i].score = score;
                // Re-heapify since score changed
                if (heap[i].score < heap[parent(i)].score) {
                    heapifyUp(i);
                } else {
                    heapifyDown(i);
                }
            }
            saveToFile();
            return;
        }
    }
    
    // If player doesn't exist and we have space, add them
    if (size < LEADERBOARD_SIZE) {
        cout << "DEBUG: Adding new player (space available): " << username << endl;
        heap[size] = LeaderboardEntry(username, score, playerID);
        size++;
        heapifyUp(size - 1);
    }
    // If player doesn't exist but has higher score than minimum, replace
    else if (score > getMinScore()) {
        cout << "DEBUG: Replacing min score with: " << username << " (" << score << ")" << endl;
        heap[0] = LeaderboardEntry(username, score, playerID);
        heapifyDown(0);
    } else {
        cout << "DEBUG: Score " << score << " not high enough for leaderboard. Min: " << getMinScore() << endl;
    }
    
    saveToFile();
}

int Leaderboard::getMinScore() const {
    return size > 0 ? heap[0].score : 0;
}

void Leaderboard::getSortedEntries(LeaderboardEntry sorted[]) const {
    // Copy heap to sorted array
    for (int i = 0; i < size; i++) {
        sorted[i] = heap[i];
    }
    
    // Simple bubble sort for display (since we only have max 10 elements)
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (sorted[j].score < sorted[j + 1].score) {
                LeaderboardEntry temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
}

bool Leaderboard::loadFromFile() {
    ifstream file("leaderboard.txt");
    if (!file.is_open()) {
        cout << "DEBUG: leaderboard.txt not found, starting fresh" << endl;
        return false;
    }
    
    size = 0;
    string line;
    
    cout << "DEBUG: Loading leaderboard from file..." << endl;
    
    while (getline(file, line) && size < LEADERBOARD_SIZE) {
        if (line.empty()) continue;
        
        // Remove any carriage return characters
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        
        if (pos1 != string::npos && pos2 != string::npos) {
            string username = line.substr(0, pos1);
            string scoreStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string playerIDStr = line.substr(pos2 + 1);
            
            // Convert strings to integers safely
            try {
                int score = stoi(scoreStr);
                int playerID = stoi(playerIDStr);
                
                heap[size] = LeaderboardEntry(username, score, playerID);
                size++;
                cout << "DEBUG: Loaded - " << username << ": " << score << endl;
            } catch (const exception& e) {
                cout << "DEBUG: Error parsing line: " << line << endl;
            }
        } else {
            cout << "DEBUG: Invalid line format: " << line << endl;
        }
    }
    
    file.close();
    
    // Build heap from loaded data
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapifyDown(i);
    }
    
    cout << "DEBUG: Loaded " << size << " entries from leaderboard.txt" << endl;
    return true;
}

bool Leaderboard::saveToFile() {
    ofstream file("leaderboard.txt");
    if (!file.is_open()) {
        cout << "DEBUG: ERROR - Cannot open leaderboard.txt for writing" << endl;
        return false;
    }
    
    LeaderboardEntry sorted[LEADERBOARD_SIZE];
    getSortedEntries(sorted);
    
    cout << "DEBUG: Saving " << size << " entries to leaderboard.txt" << endl;
    
    for (int i = 0; i < size; i++) {
        file << sorted[i].username << "|" << sorted[i].score << "|" << sorted[i].playerID << "\n";
        cout << "DEBUG: Saved - " << sorted[i].username << "|" << sorted[i].score << "|" << sorted[i].playerID << endl;
    }
    
    file.close();
    cout << "DEBUG: Leaderboard saved successfully" << endl;
    return true;
}

void Leaderboard::display() const {
    LeaderboardEntry sorted[LEADERBOARD_SIZE];
    getSortedEntries(sorted);
    
    cout << "=== LEADERBOARD ===" << endl;
    for (int i = 0; i < size; i++) {
        cout << (i + 1) << ". " << sorted[i].username << " (ID: " << sorted[i].playerID << ") - " << sorted[i].score << " points" << endl;
    }
    if (size == 0) {
        cout << "No entries yet." << endl;
    }
    cout << "===================" << endl;
}