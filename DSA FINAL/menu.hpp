// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#pragma once

#include "utilities.hpp"
#include "gameManager.hpp"
#include "authManager.hpp"

const int MAX_MENU_ITEMS = 6;

enum UI {
    NONE,
    MAIN,
    BACK_MAIN,
    START_UI,
    SINGLE_PLAYER,
    MULTIPLAYER,
    LEVEL_SELECT,
    EASY,
    MEDIUM,
    HARD,
    LEADERBOARD,
    THEMES,
    PROFILE,
    PROFILE_VIEW,
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
    bool isLoggedIn() const { return gameManager->isLoggedIn(); }

private:
    // Window and rendering
    RenderWindow* window;
    Font font;
    Text title;
    int fontSize = 45;
    Color bgColor;
    Color fontColor = { 255, 255, 255 };
    int theme;

    // Menu items
    MenuItem menuItems[MAX_MENU_ITEMS];
    int itemCount;
    int selectedIndex;
    UI currentMenuType;
    int currentScore;

    // Authentication
    GameManager* gameManager;
    //authManager authManager;
    Player currentPlayer;
    string username;
    string password;
    string loginError;
    string registerError;

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
    void setupProfileMenu();
    void setupProfileView();
    void setupLevelSelect();
    void setupLeaderBoard();
    void setupEndMenu();
    void switchTheme();

    // ===== Input Handling =====
    void handleTextInput(sf::Event& e);
    void handleBackspace();
    void handleCharInput(char c);
    void handleTabKey();
    UI handleEnterKey();
    UI attemptRegister();
    void handleKeyboard(sf::Event& e, UI& final_action);
    void handleMouse(sf::Event& e, UI& final_action);

    // ===== Action Processing =====
    MenuOptions processAction(UI action);

    // ===== Rendering =====
    void updateColors();
    void render();
    void renderLoginScreen();
    void renderRegisterScreen();
    void renderAuthScreen();
    void renderNormalMenu();
    void renderProfileMenu();
    void renderProfileView();
};

