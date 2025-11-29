#include "matchmaking.hpp"

MatchmakingQueue::MatchmakingQueue() : nextRoomID(1) {}

void MatchmakingQueue::addPlayer(const string& username, int score, int rank) {
    heap.push_back(QueuePlayer(username, score, rank));
    heapifyUp(heap.getSize() - 1);
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
    return heap.getSize();
}

void MatchmakingQueue::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].score > heap[parent].score) {
            // Swap elements
            QueuePlayer temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;
            index = parent;
        }
        else {
            break;
        }
    }
}

void MatchmakingQueue::heapifyDown(int index) {
    int size = heap.getSize();
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
            // Swap elements
            QueuePlayer temp = heap[index];
            heap[index] = heap[largest];
            heap[largest] = temp;
            index = largest;
        }
        else {
            break;
        }
    }
}

// ==================== Auto-Pairing System ====================

bool MatchmakingQueue::canMatch() {
    return heap.getSize() >= 2;
}

PlayerPair MatchmakingQueue::matchTopTwo() {
    if (!canMatch()) {
        return PlayerPair(); // Return invalid pair
    }

    // Extract top 2 players by score
    QueuePlayer player1 = extractMax();
    QueuePlayer player2 = extractMax();

    cout << "Matched: " << player1.username << " (Score: " << player1.score << ") vs "
        << player2.username << " (Score: " << player2.score << ")" << endl;

    return PlayerPair(player1, player2);
}

void MatchmakingQueue::autoMatch() {
    while (canMatch()) {
        PlayerPair matchedPair = matchTopTwo();
        if (matchedPair.isValid()) {
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
    GameRoom room = waitingRooms.getFront();
    waitingRooms.pop();

    // Move to active rooms when retrieved
    activeRooms.push_back(room);
    return room;
}

bool MatchmakingQueue::hasWaitingRooms() {
    return !waitingRooms.empty();
}

int MatchmakingQueue::getRoomCount() {
    return waitingRooms.size();
}

// ==================== Active Room Management ====================

void MatchmakingQueue::startGame(int roomID) {
    for (int i = 0; i < activeRooms.getSize(); i++) {
        if (activeRooms[i].roomID == roomID) {
            activeRooms[i].startGame();
            return;
        }
    }
    cout << "Room " << roomID << " not found for starting game." << endl;
}

void MatchmakingQueue::endGame(int roomID, int player1Score, int player2Score) {
    for (int i = 0; i < activeRooms.getSize(); i++) {
        if (activeRooms[i].roomID == roomID) {
            activeRooms[i].endGame(player1Score, player2Score);

            // Update leaderboard scores based on game results
            // This would typically update the actual leaderboard
            cout << "Winner: " << activeRooms[i].getWinner() << endl;
            return;
        }
    }
    cout << "Room " << roomID << " not found for ending game." << endl;
}

GameRoom* MatchmakingQueue::findRoomByPlayer(const string& username) {
    for (int i = 0; i < activeRooms.getSize(); i++) {
        if (activeRooms[i].isPlayerInRoom(username)) {
            return &activeRooms[i];
        }
    }
    return nullptr;
}

Vector<GameRoom> MatchmakingQueue::getActiveRooms() {
    return activeRooms;
}

void MatchmakingQueue::cleanupCompletedRooms() {
    Vector<GameRoom> stillActive;
    for (int i = 0; i < activeRooms.getSize(); i++) {
        if (activeRooms[i].active) {
            stillActive.push_back(activeRooms[i]);
        }
        else {
            cout << "Cleaning up completed room: " << activeRooms[i].roomID << endl;
        }
    }
    activeRooms = stillActive;
}

// ==================== Utility ====================

QueuePlayer MatchmakingQueue::findMatch(int targetScore, int tolerance) {
    int bestDiff = tolerance + 1;
    int bestIndex = -1;
    int size = heap.getSize();

    for (int i = 0; i < size; i++) {
        int diff = abs(heap[i].score - targetScore);
        if (diff <= tolerance && diff < bestDiff) {
            bestDiff = diff;
            bestIndex = i;
        }
    }

    if (bestIndex != -1) {
        QueuePlayer match = heap[bestIndex];

        // Remove the matched player from heap
        heap[bestIndex] = heap.back();
        heap.pop_back();

        if (bestIndex < heap.getSize()) {
            heapifyDown(bestIndex);
            heapifyUp(bestIndex);
        }

        return match;
    }

    return QueuePlayer();
}

Vector<QueuePlayer> MatchmakingQueue::getAllPlayers() {
    return heap;
}