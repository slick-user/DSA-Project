#include "authManager.hpp"

AuthManager::AuthManager() : userCount(0), loggedIn(false), nextPlayerID(1) {
    usersFile = "users.txt";
    loadUsers();
}

AuthManager::~AuthManager() {
    saveUsers();
}

int AuthManager::stringLength(const char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

bool AuthManager::compareStrings(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

void AuthManager::copyString(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (src[i] != '\0' && i < maxLen - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int AuthManager::findUserByUsername(const char* username) {
    for (int i = 0; i < userCount; i++) {
        if (compareStrings(users[i].username, username)) {
            return i;
        }
    }
    return -1;
}

bool AuthManager::isValidUsername(const char* username) {
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

bool AuthManager::isValidPassword(const char* password) {
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

void AuthManager::hashPassword(const char* password, char* hashedPassword, int maxLen) {
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

void AuthManager::getCurrentDate(char* dateStr, int maxLen) {
    time_t now = time(0);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(dateStr, maxLen, "%Y-%m-%d %H:%M:%S", &timeinfo);
}

int AuthManager::generatePlayerID() {
    return nextPlayerID++;
}

bool AuthManager::isUsernameTaken(const string& username) {
    return findUserByUsername(username.c_str()) != -1;
}

bool AuthManager::registerUser(const string& user, const string& password,
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

bool AuthManager::login(const string& user, const string& password) {
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


void AuthManager::logout() {
    loggedIn = false;
    currentPlayer = Player();
}

bool AuthManager::loadUsers() {
    std::ifstream file(usersFile);
    if (!file.is_open()) {
        return false;
    }
    
    userCount = 0;
    char line[500];
    
    while (file.getline(line, 500) && userCount < MAX_USERS) {
        if (line[0] == '\0') continue;
        
        Player player;
        int field = 0;
        int lineIdx = 0;
        char currentField[200];
        int fieldIdx = 0;
        
        // Parse: playerID|username|password|nickname|email|registrationDate
        while (line[lineIdx] != '\0' && field < 6) {
            if (line[lineIdx] == '|' || line[lineIdx] == '\n') {
                currentField[fieldIdx] = '\0';
                
                switch (field) {
                case 0:
                    player.playerID = atoi(currentField);
                    if (player.playerID >= nextPlayerID) {
                        nextPlayerID = player.playerID + 1;
                    }
                    break;
                case 1:
                    copyString(player.username, currentField, MAX_USERNAME_LEN);
                    break;
                case 2:
                    copyString(player.password, currentField, MAX_PASSWORD_LEN);
                    break;
                case 3:
                    copyString(player.nickname, currentField, MAX_NICKNAME_LEN);
                    break;
                case 4:
                    copyString(player.email, currentField, MAX_EMAIL_LEN);
                    break;
                case 5:
                    copyString(player.registrationDate, currentField, 50);
                    break;
                }
                
                field++;
                fieldIdx = 0;
            } else {
                currentField[fieldIdx++] = line[lineIdx];
            }
            lineIdx++;
        }
        
        users[userCount] = player;
        userCount++;
    }
    
    file.close();
    return true;
}

bool AuthManager::saveUsers() {
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
            << users[i].wins << "|"            
            << users[i].losses << "\n"
            << "\n";
    }

    file.close();
    return true;
}

void AuthManager::updatePlayerStats() {
    if (!loggedIn) return;

    // Update in users array
    for (int i = 0; i < userCount; i++) {
        if (users[i].playerID == currentPlayer.playerID) {
            users[i] = currentPlayer;
            break;
        }
    }

    saveUsers();
}

