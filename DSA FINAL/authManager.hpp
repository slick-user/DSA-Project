#pragma once

#include "utilities.hpp"

const int MAX_USERS = 100;
const int MAX_USERNAME_LEN = 50;
const int MAX_PASSWORD_LEN = 100;
const int MAX_NICKNAME_LEN = 50;
const int MAX_EMAIL_LEN = 100;

struct Player {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];  // Hashed
    char nickname[MAX_NICKNAME_LEN];
    char email[MAX_EMAIL_LEN];
    char registrationDate[50];
    int playerID;
    
    Player() {
        memset(username, 0, MAX_USERNAME_LEN);
        memset(password, 0, MAX_PASSWORD_LEN);
        memset(nickname, 0, MAX_NICKNAME_LEN);
        memset(email, 0, MAX_EMAIL_LEN);
        memset(registrationDate, 0, 50);
        playerID = 0;
    }
};

class authManager {
private:
    Player users[MAX_USERS];
    int userCount;
    Player currentPlayer;
    bool loggedIn;
    const char* usersFile;
    int nextPlayerID;
    
    // Helper functions
    int stringLength(const char* str);
    bool compareStrings(const char* str1, const char* str2);
    void copyString(char* dest, const char* src, int maxLen);
    int findUserByUsername(const char* username);
    void hashPassword(const char* password, char* hashedPassword, int maxLen);
    void getCurrentDate(char* dateStr, int maxLen);
    int generatePlayerID();
    bool isValidUsername(const char* username);
    bool isValidPassword(const char* password);
    
public:
    authManager();
    ~authManager();
    
    // Registration
    bool registerUser(const string& user, const string& password, 
                     const string& nickname = "", const string& email = "");
    
    // Login
    bool login(const string& user, const string& password);
    
    // Validation
    bool isUsernameTaken(const string& username);
    
    // Current user
    Player getPlayer(const string& username);
    Player getCurrentPlayer() const { return currentPlayer; }
    bool isLoggedIn() const { return loggedIn; }
    void logout();
    
    // File operations
    bool loadUsers();
    bool saveUsers();
};

