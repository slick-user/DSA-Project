// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#include "authManager.hpp"

authManager::authManager() : userCount(0), loggedIn(false), 
    usersFile("users.txt"), nextPlayerID(1) {
    loadUsers();
}

authManager::~authManager() {
    saveUsers();
}

int authManager::stringLength(const char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

bool authManager::compareStrings(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

void authManager::copyString(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (src[i] != '\0' && i < maxLen - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int authManager::findUserByUsername(const char* username) {
    for (int i = 0; i < userCount; i++) {
        if (compareStrings(users[i].username, username)) {
            return i;
        }
    }
    return -1;
}

bool authManager::isValidUsername(const char* username) {
    int len = stringLength(username);
    if (len < 3 || len > 20) {
        return false;
    }
    
    for (int i = 0; i < len; i++) {
        if (!isalnum(username[i]) && username[i] != '_') {
            return false;
        }
    }
    
    return true;
}

bool authManager::isValidPassword(const char* password) {
    int len = stringLength(password);
    if (len < 6) {
        return false;
    }
    
    bool hasLetter = false;
    bool hasNumber = false;
    
    for (int i = 0; i < len; i++) {
        if (isalpha(password[i])) hasLetter = true;
        if (isdigit(password[i])) hasNumber = true;
    }
    
    return hasLetter && hasNumber;
}

void authManager::hashPassword(const char* password, char* hashedPassword, int maxLen) {
    unsigned int hash = 0;
    int len = stringLength(password);
    
    for (int i = 0; i < len; i++) {
        hash = hash * 31 + password[i];
    }
    
    // Convert to string
    char temp[50];
    int tempIdx = 0;
    unsigned int num = hash;
    
    if (num == 0) {
        temp[tempIdx++] = '0';
    } else {
        char reverse[50];
        int revIdx = 0;
        while (num > 0) {
            reverse[revIdx++] = '0' + (num % 10);
            num /= 10;
        }
        for (int i = revIdx - 1; i >= 0; i--) {
            temp[tempIdx++] = reverse[i];
        }
    }
    temp[tempIdx] = '\0';
    
    copyString(hashedPassword, temp, maxLen);
}

void authManager::getCurrentDate(char* dateStr, int maxLen) {
    time_t now = time(0);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(dateStr, maxLen, "%Y-%m-%d %H:%M:%S", &timeinfo);
}

int authManager::generatePlayerID() {
    return nextPlayerID++;
}

bool authManager::isUsernameTaken(const string& username) {
    return findUserByUsername(username.c_str()) != -1;
}

bool authManager::registerUser(const string& user, const string& password,
                               const string& nickname, const string& email) {
    const char* username = user.c_str();
    const char* pass = password.c_str();
    
    // Validate username
    if (!isValidUsername(username)) {
        return false;
    }
    
    // Check if username is taken
    if (isUsernameTaken(user)) {
        return false;
    }
    
    // Validate password
    if (!isValidPassword(pass)) {
        return false;
    }
    
    // Check if we have space
    if (userCount >= MAX_USERS) {
        return false;
    }
    
    // Create new player
    Player newPlayer;
    copyString(newPlayer.username, username, MAX_USERNAME_LEN);
    hashPassword(pass, newPlayer.password, MAX_PASSWORD_LEN);
    
    if (nickname.length() > 0) {
        copyString(newPlayer.nickname, nickname.c_str(), MAX_NICKNAME_LEN);
    } else {
        copyString(newPlayer.nickname, username, MAX_NICKNAME_LEN);
    }
    
    if (email.length() > 0) {
        copyString(newPlayer.email, email.c_str(), MAX_EMAIL_LEN);
    } else {
        newPlayer.email[0] = '\0';
    }
    
    getCurrentDate(newPlayer.registrationDate, 50);
    newPlayer.playerID = generatePlayerID();
    
    // Add to users array
    users[userCount] = newPlayer;
    userCount++;
    
    // Save to file
    return saveUsers();
}

bool authManager::login(const string& user, const string& password) {
    int userIndex = findUserByUsername(user.c_str());
    if (userIndex == -1) {
        return false;
    }
    
    char hashedPassword[MAX_PASSWORD_LEN];
    hashPassword(password.c_str(), hashedPassword, MAX_PASSWORD_LEN);
    
    if (compareStrings(users[userIndex].password, hashedPassword)) {
        currentPlayer = users[userIndex];
        loggedIn = true;
        return true;
    }
    
    return false;
}

Player authManager::getPlayer(const string& username) {
    int index = findUserByUsername(username.c_str());
    if (index != -1) {
        return users[index];
    }
    return Player();
}

void authManager::logout() {
    loggedIn = false;
    currentPlayer = Player();
}

bool authManager::loadUsers() {
    std::ifstream file(usersFile);
    if (!file.is_open()) {
        return false;
    }

    userCount = 0;
    std::string line;
    while (std::getline(file, line) && userCount < MAX_USERS) {
        if (line.empty()) continue;

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        std::istringstream ss(line);
        std::string field;
        Player player;

        // Updated fields with game statistics
        // playerID|username|password|nickname|email|registrationDate|totalGames|totalPoints|wins|losses

        // Field 0: playerID
        if (!getline(ss, field, '|')) continue;
        player.playerID = std::atoi(field.c_str());
        if (player.playerID >= nextPlayerID) {
            nextPlayerID = player.playerID + 1;
        }

        // Field 1: username
        if (!getline(ss, field, '|')) continue;
        copyString(player.username, field.c_str(), MAX_USERNAME_LEN);

        // Field 2: password
        if (!getline(ss, field, '|')) continue;
        copyString(player.password, field.c_str(), MAX_PASSWORD_LEN);

        // Field 3: nickname
        if (!getline(ss, field, '|')) {
            copyString(player.nickname, player.username, MAX_NICKNAME_LEN);
        }
        else {
            copyString(player.nickname, field.c_str(), MAX_NICKNAME_LEN);
        }

        // Field 4: email
        if (!getline(ss, field, '|')) {
            player.email[0] = '\0';
        }
        else {
            copyString(player.email, field.c_str(), MAX_EMAIL_LEN);
        }

        // Field 5: registrationDate
        if (!getline(ss, field, '|')) {
            player.registrationDate[0] = '\0';
        }
        else {
            copyString(player.registrationDate, field.c_str(), 50);
        }

        // Field 6: totalGames
        if (!getline(ss, field, '|')) {
            player.totalGames = 0;
        }
        else {
            player.totalGames = std::atoi(field.c_str());
        }

        // Field 7: totalPoints
        if (!getline(ss, field, '|')) {
            player.totalPoints = 0;
        }
        else {
            player.totalPoints = std::atoi(field.c_str());
        }

        /*
        // Field 8: wins 
        if (!getline(ss, field, '|')) {
            player.wins = 0;
        }
        else {
            player.wins = std::atoi(field.c_str());
        }

        // Field 9: losses - read rest of line
        if (!getline(ss, field)) {
            player.losses = 0;
        }
        else {
            player.losses = std::atoi(field.c_str());
        }

        */

        users[userCount++] = player;
    }

    file.close();
    return true;
}

bool authManager::saveUsers() {
    ofstream file(usersFile);
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < userCount; i++) {
        file << users[i].playerID << "|"
            << users[i].username << "|"
            << users[i].password << "|"
            << users[i].nickname << "|"
            << users[i].email << "|"
            << users[i].registrationDate << "|"
            << users[i].totalGames << "|"
            << users[i].totalPoints << "|"
            //<< users[i].wins << "|"            
            //<< users[i].losses << "\n";  
            << "\n";
    }

    file.close();
    return true;
}

void authManager::updatePlayerStats(int score) {
    if (!loggedIn) return;

    // Update current player
    currentPlayer.totalGames++;
    currentPlayer.totalPoints += score;

    // Update in users array
    for (int i = 0; i < userCount; i++) {
        if (users[i].playerID == currentPlayer.playerID) {
            users[i] = currentPlayer;
            break;
        }
    }

    saveUsers();
}