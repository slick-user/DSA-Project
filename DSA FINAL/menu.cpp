#include "menu.hpp"

// ===================== Constructor =====================

Menu::Menu(RenderWindow* window) : window(window), currentScore(0),
usernameLen(0), passwordLen(0), nicknameLen(0), emailLen(0),
currentInputField(0), isInputMode(false), theme(0) {
    bgColor = Color(0, 0, 0);
    fontColor = Color(255, 255, 255);
    memset(usernameInput, 0, 100);
    memset(passwordInput, 0, 100);
    memset(nicknameInput, 0, 100);
    memset(emailInput, 0, 100);
}

// ===================== Helper Functions =====================

void Menu::clearMenuItems() {
    for (int i = 0; i < MAX_MENU_ITEMS; ++i) {
        menuItems[i].text.setString("");
        menuItems[i].action = NONE;
        menuItems[i].bounds = {};
    }
}

void Menu::setItem(int index, const string& label, float x, float y, UI action) {
    menuItems[index].text.setFont(font);
    menuItems[index].text.setString(label);
    menuItems[index].text.setCharacterSize(fontSize);
    menuItems[index].text.setPosition(x, y);
    menuItems[index].action = action;
}

void Menu::finalizeBounds(int startIndex) {
    for (int i = startIndex; i < itemCount; ++i) {
        if (menuItems[i].action != NONE)
            menuItems[i].bounds = menuItems[i].text.getGlobalBounds();
    }
}

void Menu::resetAuthInputs() {
    memset(usernameInput, 0, 100);
    memset(passwordInput, 0, 100);
    memset(nicknameInput, 0, 100);
    memset(emailInput, 0, 100);
    usernameLen = passwordLen = nicknameLen = emailLen = 0;
    currentInputField = 0;
    loginError.clear();
    registerError.clear();
    isInputMode = true;
}

bool Menu::isMouseOver(const sf::FloatRect& bounds, int mouseX, int mouseY) const {
    return bounds.contains(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

void Menu::setScore(int score) {
    currentScore = score;
}

// ===================== Menu Setup Functions =====================

void Menu::setupAuthScreen() {
    currentMenuType = AUTH_SCREEN;
    itemCount = 3;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Login", 150, 150, LOGIN_MENU);
    setItem(1, "Register", 150, 220, REGISTER_MENU);
    setItem(2, "Exit", 150, 290, QUIT);

    finalizeBounds(0);
    isInputMode = false;
}

void Menu::setupLoginMenu() {
    currentMenuType = LOGIN_MENU;
    itemCount = 3;
    selectedIndex = 0;
    clearMenuItems();

    //setItem(1, "Login", 150, 200, LOGIN);
    setItem(2, "Back", 150, 270, AUTH_SCREEN);

    finalizeBounds(1);
    resetAuthInputs();
}

void Menu::setupRegisterMenu() {
    currentMenuType = REGISTER_MENU;
    itemCount = 2;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Register", 180, 50, NONE);
    setItem(1, "Back", 150, 400, AUTH_SCREEN);

    finalizeBounds(1);
    resetAuthInputs();
}

void Menu::setupMainMenu() {
    currentMenuType = MAIN;
    itemCount = 5;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Play", 150, 80, START_UI);
    setItem(1, "Select Level", 150, 150, LEVEL_SELECT);
    setItem(2, "LeaderBoard", 150, 220, LEADERBOARD);
    setItem(3, "Themes", 150, 290, THEMES);
    setItem(4, "Exit", 150, 360, QUIT);

    finalizeBounds(0);
}

void Menu::setupStartMenu() {
    currentMenuType = START_UI;
    itemCount = 3;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "SinglePlayer", 150, 150, SINGLE_PLAYER);
    setItem(1, "MultiPlayer", 150, 220, MULTIPLAYER);
    setItem(2, "Back", 150, 290, BACK_MAIN);

    finalizeBounds(0);
}

void Menu::setupLevelSelect() {
    currentMenuType = LEVEL_SELECT;
    itemCount = 4;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Level 1 Easy", 150, 150, EASY);
    setItem(1, "Level 2 Medium", 150, 220, MEDIUM);
    setItem(2, "Level 3 Hard", 150, 290, HARD);
    setItem(3, "Back", 150, 360, BACK_MAIN);

    finalizeBounds(0);
}

void Menu::setupLeaderBoard() {
    // To be implemented
}

void Menu::setupEndMenu() {
    currentMenuType = END;
    itemCount = 5;
    selectedIndex = 2;
    clearMenuItems();

    setItem(0, "Game Over", 170, 80, NONE);
    setItem(1, "Score: " + std::to_string(currentScore), 150, 150, NONE);
    setItem(2, "Restart", 150, 220, SINGLE_PLAYER);
    setItem(3, "Main Menu", 150, 290, BACK_MAIN);
    setItem(4, "Exit Game", 150, 360, QUIT);

    finalizeBounds(2);
}

void Menu::switchTheme() {
    theme = (theme + 1) % 8;

    static const Color themes[] = {
        {0,0,0}, {255,255,255}, {0,0,255}, {255,0,0},
        {255,0,255}, {0,0,155}, {0,155,0}, {155,0,0}
    };

    bgColor = themes[theme];
    fontColor = (theme == 1) ? Color::Black : Color::White;
}

// ===================== Input Handling =====================

void Menu::handleTextInput(sf::Event& e) {
    if (!isInputMode || (currentMenuType != LOGIN_MENU && currentMenuType != REGISTER_MENU))
        return;

    if (e.type == Event::TextEntered) {
        if (e.text.unicode == '\b') {
            handleBackspace();
        }
        else if (e.text.unicode >= 32 && e.text.unicode < 127) {
            handleCharInput(static_cast<char>(e.text.unicode));
        }
    }
}

void Menu::handleBackspace() {
    if (currentMenuType == LOGIN_MENU) {
        if (currentInputField == 0 && usernameLen > 0) usernameInput[--usernameLen] = '\0';
        else if (currentInputField == 1 && passwordLen > 0) passwordInput[--passwordLen] = '\0';
    }
    else if (currentMenuType == REGISTER_MENU) {
        if (currentInputField == 0 && usernameLen > 0) usernameInput[--usernameLen] = '\0';
        else if (currentInputField == 1 && passwordLen > 0) passwordInput[--passwordLen] = '\0';
        else if (currentInputField == 2 && nicknameLen > 0) nicknameInput[--nicknameLen] = '\0';
        else if (currentInputField == 3 && emailLen > 0) emailInput[--emailLen] = '\0';
    }
}

void Menu::handleCharInput(char c) {
    if (currentMenuType == LOGIN_MENU) {
        if (currentInputField == 0 && usernameLen < 99) {
            usernameInput[usernameLen++] = c;
            usernameInput[usernameLen] = '\0';
        }
        else if (currentInputField == 1 && passwordLen < 99) {
            passwordInput[passwordLen++] = c;
            passwordInput[passwordLen] = '\0';
        }
    }
    else if (currentMenuType == REGISTER_MENU) {
        if (currentInputField == 0 && usernameLen < 99) {
            usernameInput[usernameLen++] = c;
            usernameInput[usernameLen] = '\0';
        }
        else if (currentInputField == 1 && passwordLen < 99) {
            passwordInput[passwordLen++] = c;
            passwordInput[passwordLen] = '\0';
        }
        else if (currentInputField == 2 && nicknameLen < 99) {
            nicknameInput[nicknameLen++] = c;
            nicknameInput[nicknameLen] = '\0';
        }
        else if (currentInputField == 3 && emailLen < 99) {
            emailInput[emailLen++] = c;
            emailInput[emailLen] = '\0';
        }
    }
}

void Menu::handleTabKey() {
    if (currentMenuType == LOGIN_MENU) {
        currentInputField = (currentInputField + 1) % 2;
    }
    else if (currentMenuType == REGISTER_MENU) {
        currentInputField = (currentInputField + 1) % 4;
    }
}

UI Menu::handleEnterKey() {
    if (currentMenuType == LOGIN_MENU) {
        if (currentInputField == 0 && usernameLen > 0) {
            currentInputField = 1;
        }
        else if (currentInputField == 1 && passwordLen > 0) {
            username = string(usernameInput);
            password = string(passwordInput);
            return LOGIN;
        }
    }
    else if (currentMenuType == REGISTER_MENU) {
        if (currentInputField < 3) {
            currentInputField++;
        }
        else {
            return attemptRegister();
        }
    }
    return MAIN;
}

UI Menu::attemptRegister() {
    username = string(usernameInput);
    password = string(passwordInput);
    string nickname = string(nicknameInput);
    string email = string(emailInput);

    if (usernameLen == 0 || passwordLen == 0) {
        registerError = "Username and password required!";
    }
    else if (passwordLen < 6) registerError = "Password must be 6 characters or greater";

    else if (authManager.isUsernameTaken(username)) {
        registerError = "Username already taken!";
    }
    else {
        if (authManager.registerUser(username, password, nickname, email)) {
            authManager.login(username, password);
            currentPlayer = authManager.getCurrentPlayer();
            return MAIN;
        }
        else {
            registerError = "Registration failed! Password must contain atleast one letter and number";
        }
    }
    return MAIN;
}

void Menu::handleKeyboard(sf::Event& e, UI& final_action) {
    if (e.type != Event::KeyPressed) return;

    // Handle input mode keys
    if (isInputMode && (currentMenuType == LOGIN_MENU || currentMenuType == REGISTER_MENU)) {
        if (e.key.code == Keyboard::Tab) {
            handleTabKey();
        }
        else if (e.key.code == Keyboard::Enter) {
            final_action = handleEnterKey();
        }
        else if (e.key.code == Keyboard::Escape) {
            setupAuthScreen();
        }
        return;
    }

    // Handle menu navigation
    if (e.key.code == Keyboard::Up || e.key.code == Keyboard::W) {
        do {
            selectedIndex = (selectedIndex + itemCount - 1) % itemCount;
        } while (menuItems[selectedIndex].action == NONE && itemCount > 1);
    }
    else if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S) {
        do {
            selectedIndex = (selectedIndex + 1) % itemCount;
        } while (menuItems[selectedIndex].action == NONE && itemCount > 1);
    }
    else if (e.key.code == Keyboard::Enter || e.key.code == Keyboard::Space) {
        if (menuItems[selectedIndex].action != NONE) {
            final_action = static_cast<UI>(menuItems[selectedIndex].action);
        }
    }
}

void Menu::handleMouse(sf::Event& e, UI& final_action) {
    if (e.type == Event::MouseMoved) {
        int mouseX = e.mouseMove.x;
        int mouseY = e.mouseMove.y;

        for (int i = 0; i < itemCount; ++i) {
            if (menuItems[i].action != NONE && isMouseOver(menuItems[i].bounds, mouseX, mouseY)) {
                selectedIndex = i;
                break;
            }
        }
    }
    else if (e.type == Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        int mouseX = e.mouseButton.x;
        int mouseY = e.mouseButton.y;

        for (int i = 0; i < itemCount; ++i) {
            if (menuItems[i].action != NONE && isMouseOver(menuItems[i].bounds, mouseX, mouseY)) {
                selectedIndex = i;
                final_action = static_cast<UI>(menuItems[selectedIndex].action);
                break;
            }
        }
    }
}

MenuOptions Menu::processAction(UI action) {
    switch (action) {
    case SINGLE_PLAYER:
        return PLAY;
    case LEVEL_SELECT:
        setupLevelSelect();
        break;
    case LEADERBOARD:
        setupLeaderBoard();
        break;
    case QUIT:
        return EXIT;
    case START_UI:
        setupStartMenu();
        break;
    case THEMES:
        switchTheme();
        break;
    case LOGIN_MENU:
        setupLoginMenu();
        break;
    case LOGIN:
        if (authManager.login(username, password)) {
            currentPlayer = authManager.getCurrentPlayer();
            isInputMode = false;
            cout << "works!";
            setupMainMenu();
        }
        else {
            loginError = "Invalid Credentials!";
        }
        break;
    case REGISTER_MENU:
        setupRegisterMenu();
        break;
    case BACK_MAIN:
        setupMainMenu();
        break;
    case END:
        setupEndMenu();
        break;
    default:
        break;
    }
    return MENU;
}

// ===================== Rendering =====================

void Menu::updateColors() {
    for (int i = 0; i < itemCount; ++i) {
        menuItems[i].text.setFillColor(i == selectedIndex ? Color::Yellow : fontColor);
    }
}

void Menu::renderLoginScreen() {
    Text title("Login", font, fontSize);
    title.setPosition(200, 30);
    title.setFillColor(fontColor);
    window->draw(title);

    Text userLabel("Username: " + string(usernameInput), font, 30);
    userLabel.setPosition(100, 120);
    userLabel.setFillColor(currentInputField == 0 ? Color::Yellow : fontColor);
    window->draw(userLabel);

    string passDisplay(passwordLen, '*');
    Text passLabel("Password: " + passDisplay, font, 30);
    passLabel.setPosition(100, 160);
    passLabel.setFillColor(currentInputField == 1 ? Color::Yellow : fontColor);
    window->draw(passLabel);

    if (!loginError.empty()) {
        Text error(loginError, font, 25);
        error.setPosition(100, 210);
        error.setFillColor(Color::Red);
        window->draw(error);
    }

    Text instruction("Tab: Switch | Enter: Login | Esc: Back", font, 20);
    instruction.setPosition(50, 260);
    instruction.setFillColor(fontColor);
    window->draw(instruction);

    // Draw menu items (back button)
    for (int i = 1; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
    }
}

void Menu::renderRegisterScreen() {
    Text title("Register", font, fontSize);
    title.setPosition(180, 30);
    title.setFillColor(fontColor);
    window->draw(title);

    Text userLabel("Username: " + string(usernameInput), font, 25);
    userLabel.setPosition(80, 100);
    userLabel.setFillColor(currentInputField == 0 ? Color::Yellow : fontColor);
    window->draw(userLabel);

    string passDisplay(passwordLen, '*');
    Text passLabel("Password: " + passDisplay, font, 25);
    passLabel.setPosition(80, 140);
    passLabel.setFillColor(currentInputField == 1 ? Color::Yellow : fontColor);
    window->draw(passLabel);

    Text nickLabel("Nickname (optional): " + string(nicknameInput), font, 25);
    nickLabel.setPosition(80, 180);
    nickLabel.setFillColor(currentInputField == 2 ? Color::Yellow : fontColor);
    window->draw(nickLabel);

    Text emailLabel("Email (optional): " + string(emailInput), font, 25);
    emailLabel.setPosition(80, 220);
    emailLabel.setFillColor(currentInputField == 3 ? Color::Yellow : fontColor);
    window->draw(emailLabel);

    if (!registerError.empty()) {
        Text error(registerError, font, 20);
        error.setPosition(80, 270);
        error.setFillColor(Color::Red);
        window->draw(error);
    }

    Text instruction("Tab: Next | Enter: Register | Esc: Back", font, 18);
    instruction.setPosition(50, 320);
    instruction.setFillColor(fontColor);
    window->draw(instruction);

    // Draw back button
    window->draw(menuItems[1].text);
}

void Menu::renderAuthScreen() {
    Text title("Welcome to Game", font, fontSize);
    title.setPosition(150, 50);
    title.setFillColor(fontColor);
    window->draw(title);

    for (int i = 0; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
    }
}

void Menu::renderNormalMenu() {
    for (int i = 0; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
    }
}

void Menu::render() {
    window->clear(bgColor);

    if (currentMenuType == LOGIN_MENU) {
        renderLoginScreen();
    }
    else if (currentMenuType == REGISTER_MENU) {
        renderRegisterScreen();
    }
    else if (currentMenuType == AUTH_SCREEN) {
        renderAuthScreen();
    }
    else {
        renderNormalMenu();
    }

    window->display();
}

// ===================== Main Menu Loop =====================

MenuOptions Menu::runMainMenu(bool m) {
    if (!font.loadFromFile("assets/monogram.ttf")) {
        return EXIT;
    }

    if (!authManager.isLoggedIn() && m) {
        setupLoginMenu();
    }
    else if (m) {
        setupMainMenu();
    }
    else {
        setupEndMenu();
    }

    while (window->isOpen()) {
        UI final_action = MAIN;

        Event e;
        while (window->pollEvent(e)) {
            if (e.type == Event::Closed) {
                window->close();
                return EXIT;
            }

            handleTextInput(e);
            handleKeyboard(e, final_action);
            handleMouse(e, final_action);
        }

        if (final_action != MAIN) {
            MenuOptions result = processAction(final_action);
            if (result != MENU) return result;
        }

        updateColors();
        render();
    }

    return EXIT;
}

MenuOptions Menu::runAuthScreen() {
    if (!font.loadFromFile("assets/monogram.ttf")) {
        return EXIT;
    }

    setupAuthScreen();

    while (window->isOpen()) {
        UI final_action = MAIN;

        Event e;
        while (window->pollEvent(e)) {
            if (e.type == Event::Closed) {
                window->close();
                return EXIT;
            }

            handleTextInput(e);
            handleKeyboard(e, final_action);
            handleMouse(e, final_action);
        }

        if (final_action != MAIN) {
            MenuOptions result = processAction(final_action);
            if (result != MENU) return result;
        }

        updateColors();
        render();
    }

    return EXIT;
}