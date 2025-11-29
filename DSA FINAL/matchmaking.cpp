#include "matchmaking.hpp"

MatchmakingQueue::MatchmakingQueue() : nextRoomID(1) {}

void MatchmakingQueue::addPlayer(const string& username, int score, int rank) {
    heap.push_back(QueuePlayer(username, score, rank));
    heapifyUp(heap.size() - 1);
}

void MatchmakingQueue::addPlayerFromLeaderboard(const string& username, Leaderboard& lb) {
    // Get player's score from leaderboard
    LeaderboardEntry sorted[LEADERBOARD_SIZE];
    lb.getSortedEntries(sorted);
    
    for (int i = 0; i < lb.getSize(); i++) {
        if (sorted[i].username == username) {
            addPlayer(username, sorted[i].score, i + 1); // rank is position + 1
            return;
        }
    }
    // If not found in leaderboard, add with default score
    addPlayer(username, 0, 999);
}

QueuePlayer MatchmakingQueue::extractMax() {
    if (isEmpty()) return QueuePlayer();
    
    QueuePlayer maxPlayer = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    
    if (!isEmpty()) {
        heapifyDown(0);
    }
    
    return maxPlayer;
}

QueuePlayer MatchmakingQueue::peekMax() {
    if (isEmpty()) return QueuePlayer();
    return heap[0];
}

bool MatchmakingQueue::isEmpty() {
    return heap.empty();
}

int MatchmakingQueue::getSize() {
    return heap.size();
}

void MatchmakingQueue::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].score > heap[parent].score) {
            swap(heap[index], heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void MatchmakingQueue::heapifyDown(int index) {
    int size = heap.size();
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;
        
        if (left < size && heap[left].score > heap[largest].score) {
            largest = left;
        }
        
        if (right < size && heap[right].score > heap[largest].score) {
            largest = right;
        }
        
        if (largest != index) {
            swap(heap[index], heap[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

// ==================== Auto-Pairing System ====================

bool MatchmakingQueue::canMatch() {
    return heap.size() >= 2;
}

pair<QueuePlayer, QueuePlayer> MatchmakingQueue::matchTopTwo() {
    if (!canMatch()) {
        return make_pair(QueuePlayer(), QueuePlayer());
    }
    
    // Extract top 2 players by score
    QueuePlayer player1 = extractMax();
    QueuePlayer player2 = extractMax();
    
    cout << "Matched: " << player1.username << " (Score: " << player1.score << ") vs " 
         << player2.username << " (Score: " << player2.score << ")" << endl;
    
    return make_pair(player1, player2);
}

void MatchmakingQueue::autoMatch() {
    while (canMatch()) {
        auto matchedPair = matchTopTwo();
        if (!matchedPair.first.username.empty()) {
            createRoom(matchedPair.first, matchedPair.second);
        }
    }
}

// ==================== Game Room Management ====================

int MatchmakingQueue::createRoom(QueuePlayer p1, QueuePlayer p2) {
    GameRoom room(p1, p2, nextRoomID++);
    waitingRooms.push(room);
    cout << "Game Room " << room.roomID << " created for " 
         << p1.username << " vs " << p2.username << endl;
    return room.roomID;
}

GameRoom MatchmakingQueue::getNextRoom() {
    if (waitingRooms.empty()) {
        return GameRoom();
    }
    GameRoom room = waitingRooms.front();
    waitingRooms.pop();
    return room;
}

bool MatchmakingQueue::hasWaitingRooms() {
    return !waitingRooms.empty();
}

int MatchmakingQueue::getRoomCount() {
    return waitingRooms.size();
}

// ==================== Utility ====================

QueuePlayer MatchmakingQueue::findMatch(int targetScore, int tolerance) {
    int bestDiff = tolerance + 1;
    int bestIndex = -1;
    
    for (size_t i = 0; i < heap.size(); i++) {
        int diff = abs(heap[i].score - targetScore);
        if (diff <= tolerance && diff < bestDiff) {
            bestDiff = diff;
            bestIndex = i;
        }
    }
    
    if (bestIndex != -1) {
        QueuePlayer match = heap[bestIndex];
        
        heap[bestIndex] = heap.back();
        heap.pop_back();
        
        if (bestIndex < (int)heap.size()) {
            heapifyDown(bestIndex);
            heapifyUp(bestIndex);
        }
        
        return match;
    }
    
    return QueuePlayer();
}

vector<QueuePlayer> MatchmakingQueue::getAllPlayers() {
    return heap;
}
