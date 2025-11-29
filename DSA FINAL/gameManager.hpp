// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT
#pragma once
#include "authManager.hpp"

class GameManager {
private:
    static GameManager* instance;
    AuthManager auth;

    GameManager() = default; // Private constructor

public:
    // Delete copy constructor and assignment operator
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    static GameManager* getInstance() {
        if (!instance) {
            instance = new GameManager();
        }
        return instance;
    }

    // Simple proxy methods to authManager
    bool login(const string& user, const string& pass) {
        return auth.login(user, pass);
    }

    bool registerUser(const string& user, const string& pass,
        const string& nick, const string& email) {
        return auth.registerUser(user, pass, nick, email);
    }

    void updatePlayerStats() {
        auth.updatePlayerStats();
    }

    Player getCurrentPlayer() {
        return auth.getCurrentPlayer();
    }

    Player* getCurrentsPlayer() {
        return auth.getCurrentsPlayer();
    }

    bool isLoggedIn() {
        return auth.isLoggedIn();
    }

    void logout() {
        auth.logout();
    }

    bool isUsernameTaken(const string& username) {
        return auth.isUsernameTaken(username);
    }
};

