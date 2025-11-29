#pragma once

#include "utilities.hpp"
#include "authManager.hpp"
#include "gameManager.hpp"
#include "leaderboard.hpp"
#include "avl_tree.hpp"
#include "friendManager.hpp"
#include "matchmaking.hpp"
#include "inventory.hpp"

const int MAX_MENU_ITEMS = 10;

enum UI {
    NONE,
    MAIN,
    BACK_MAIN,
    START_UI,
    LOAD_GAME_UI,
    SINGLE_PLAYER,
    MULTIPLAYER,
    LEVEL_SELECT,
    EASY_LEVEL,
    MEDIUM_LEVEL,
    HARD_LEVEL,
    LEADERBOARD_UI,
    THEMES,
    PROFILE,
    PROFILE_VIEW,
    FRIENDS_MENU,
    ADD_FRIEND,
    VIEW_FRIENDS,
    VIEW_REQUESTS,
    VIEW_SENT_REQUESTS,
    REMOVE_FRIEND,
    MATCHMAKING_MENU,
    FIND_MATCH,
    END,
    QUIT,
    AUTH_SCREEN,
    LOGIN_MENU,
    REGISTER_MENU,
    LOGIN,
    REGISTER,
};

struct MenuItem {
    Text text;
    UI action;
    FloatRect bounds;
};

class Menu {
public:
    Menu(RenderWindow* window = nullptr);
    MenuOptions runMainMenu(bool m);
    MenuOptions runAuthScreen();
    void setScore(int score);
    void setMultiplayerScores(int p1Score, int p2Score);
    bool isLoggedIn() const { return gameManager->isLoggedIn(); }
    
    // Getter methods
    string getCurrentUsername() const { return string(currentPlayer.username); }
    int getCurrentPlayerID() const { return currentPlayer.playerID; }
    Player getCurrentPlayer() const { return currentPlayer; }
    GameLevel getSelectedLevel() const { return selectedLevel; }
    GameMode getSelectedMode() const { return selectedMode; }
    
    // Second player for multiplayer
    string getPlayer2Username() const { return player2Name; }
    int getPlayer2ID() const { return player2ID; }
    
    Color getBackgroundColor() const { return bgColor; }

private:
    // Window and rendering
    RenderWindow* window;
    Font font;
    Text title;
    int fontSize = 45;
    Color bgColor;
    Color fontColor = { 255, 255, 255 };
    
    // Theme structure managed by AVL Tree
    AVLTree themeTree;
    int currentThemeIndex;
    
    // Helper to get current theme
    Theme getCurrentTheme() {
        return themeTree.getThemeByIndex(currentThemeIndex);
    }

    // Menu items
    MenuItem menuItems[MAX_MENU_ITEMS];
    int itemCount;
    int selectedIndex;
    UI currentMenuType;
    int currentScore;
    int player1Score;
    int player2Score;

    Inventory* themeInventory;
    bool showInventory; 

    // Authentication
    GameManager* gameManager;
    //AuthManager AuthManager;
    Player currentPlayer;
    string username;
    string password;
    string loginError;
    string registerError;

    // Leaderboard
    Leaderboard leaderboard;
    
    // Friend Manager
    FriendManager friendManager;
    char friendInput[100];
    int friendInputLen;
    string friendError;
    string friendSuccess;
    int selectedRequestIndex;

    // Friend management states
    bool showingFriends;
    bool showingRequests;
    bool showingSentRequests;
    
    // Matchmaking
    MatchmakingQueue matchmakingQueue;
    bool isSearching;
    Clock searchTimer;
    string searchStatus;
    
    // Level and mode selection
    GameLevel selectedLevel;
    GameMode selectedMode;
    bool lastGameWasMultiplayer;
    
    // Multiplayer - second player
    string player2Name;
    int player2ID;
    char player2Input[100];
    int player2InputLen;
    bool waitingForPlayer2;

    // Input handling
    char usernameInput[100];
    char passwordInput[100];
    char nicknameInput[100];
    char emailInput[100];
    int usernameLen, passwordLen, nicknameLen, emailLen;
    int currentInputField;
    bool isInputMode;

    // ===== Helper Functions =====
    void clearMenuItems();
    void setItem(int index, const string& label, float x, float y, UI action);
    void finalizeBounds(int startIndex = 0);
    void resetAuthInputs();
    bool isMouseOver(const FloatRect& bounds, int mouseX, int mouseY) const;

    // ===== Menu Setup =====
    void setupAuthScreen();
    void setupLoginMenu();
    void setupRegisterMenu();
    void setupMainMenu();
    void setupStartMenu();
    void setupLevelSelect();
    void setupLeaderBoard();
    void setupProfileMenu();
    void setupProfileView();
    void setupFriendsMenu();
    void setupMatchmakingMenu();
    void setupInventoryMenu();
    void setupEndMenu();
    void setupMultiplayerSetup();
    void switchTheme();

    // ===== Input Handling =====
    void handleTextInput(sf::Event& e);
    void handleBackspace();
    void handleCharInput(char c);
    void handleFriendInput(char c);
    void handleTabKey();
    UI handleEnterKey();
    UI attemptRegister();
    void attemptAddFriend();
    void startMatchmaking();
    void updateMatchmaking();
    void handleKeyboard(sf::Event& e, UI& final_action);
    void handleMouse(sf::Event& e, UI& final_action);

    // ===== Action Processing =====
    MenuOptions processAction(UI action);

    // ===== Rendering =====
    void updateColors();
    void update();
    void render();
    void renderLoginScreen();
    void renderRegisterScreen();
    void renderAuthScreen();
    void renderLeaderBoard();
    void renderFriendsMenu();
    void renderProfileMenu();
    void renderProfileView();
    void renderMatchmakingMenu();
    void renderNormalMenu();
    void renderMultiplayerSetup();
    void renderPendingRequests();
    void renderSentRequests();
    void renderAddFriendSection();
    void renderFriendsList();
};