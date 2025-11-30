#include "menu.hpp"

// Constructor 
Menu::Menu(RenderWindow* window) : window(window), currentScore(0),
            usernameLen(0), passwordLen(0), nicknameLen(0), emailLen(0),
            currentInputField(0), isInputMode(false), selectedLevel(EASY),
            selectedMode(SINGLE_PLAYER_MODE), player1Score(0), player2Score(0),
            player2Name("Player2"), player2ID(0), player2InputLen(0), waitingForPlayer2(false),
            lastGameWasMultiplayer(false), showingFriends(true), showingRequests(false),
            showingSentRequests(false), showInventory(false), selectedPlayerIndex(0), inGameRoom(false),
            matchmakingTolerance(500) {

    themeInventory = new Inventory(100, 50, 300, 300);

    // Initialize Themes in AVL Tree
    // 1. Basic
    Theme basic;
    themeTree.insert(basic);

    // 2. Cyberpunk (Modern Dark)
    Theme cyberpunk(1, "Cyberpunk",
        Color(11, 12, 16),     // bgColor
        Color(102, 252, 241),  // titleColor  
        Color(197, 198, 199),  // textColor
        Color(69, 162, 158),   // highlightColor
        Color(31, 40, 51));    // buttonColor
    themeTree.insert(cyberpunk);

    // 2. Midnight Gold (Premium)
    Theme midnight(2, "Midnight Gold",
        Color(26, 26, 46),     // bgColor
        Color(242, 169, 0),    // titleColor
        Color(233, 233, 233),  // textColor
        Color(228, 53, 53),    // highlightColor
        Color(22, 33, 62));    // buttonColor
    themeTree.insert(midnight);

    // 3. Matrix (Hacker)
    Theme matrix(3, "Matrix",
        Color(0, 0, 0),          // bgColor
        Color(0, 255, 65),       // titleColor
        Color(0, 143, 17),       // textColor
        Color(255, 255, 255),    // highlightColor
        Color(13, 2, 8));        // buttonColor
    themeTree.insert(matrix);

    // 5. Sunset Orange
    Theme sunset(5, "Sunset Orange",
        Color(44, 29, 53),       // Deep purple
        Color(255, 159, 28),     // Orange
        Color(255, 223, 184),    // Cream
        Color(234, 93, 104),     // Pink
        Color(65, 43, 79));      // Purple
    themeTree.insert(sunset);

    // 6. Forest Green
    Theme forest(6, "Forest Green",
        Color(23, 35, 24),       // Dark green
        Color(120, 224, 143),    // Light green
        Color(200, 230, 201),    // Pale green
        Color(255, 203, 107),    // Yellow
        Color(40, 62, 42));      // Medium green
    themeTree.insert(forest);

    // 7. Royal Purple
    Theme royal(7, "Royal Purple",
        Color(36, 27, 52),        // Dark purple
        Color(188, 108, 255),     // Light purple
        Color(230, 210, 255),     // Lavender
        Color(255, 168, 106),     // Peach
        Color(54, 41, 78));       // Medium purple
    themeTree.insert(royal);

    // 8. Fire Red
    Theme fire(8, "Fire Red",
        Color(40, 16, 16),         // Dark red
        Color(255, 80, 80),        // Bright red
        Color(255, 200, 200),      // Light red
        Color(255, 223, 0),        // Yellow
        Color(70, 28, 28));        // Medium red
    themeTree.insert(fire); 

    // 9. Desert
    Theme desert(9, "Desert",
        Color(40, 44, 52),
        Color(97, 175, 239),
        Color(220, 220, 220),
        Color(229, 192, 123),
        Color(56, 58, 66));
    themeTree.insert(desert);

    currentThemeIndex = 0;

    themeInventory->initialize(themeTree);

    // Set initial colors from default theme
    Theme current = getCurrentTheme();
    bgColor = current.bgColor;
    title.setFillColor(current.titleColor);
    fontColor = Color(255, 255, 255);

    //fontColor = current.textColor;
    title.setFillColor(current.titleColor);
    fontColor = Color(255, 255, 255);
    memset(usernameInput, 0, 100);
    memset(passwordInput, 0, 100);
    memset(nicknameInput, 0, 100);
    memset(emailInput, 0, 100);
    memset(player2Input, 0, 100);

    gameManager = GameManager::getInstance();
}

//  Helper Functions 

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
    lastGameWasMultiplayer = false;
}

void Menu::setMultiplayerScores(int p1Score, int p2Score) {
    player1Score = p1Score;
    player2Score = p2Score;
    lastGameWasMultiplayer = true;
}

//               Menu Setup Functions 

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

    setItem(2, "Back", 150, 270, AUTH_SCREEN);

    finalizeBounds(1);
    resetAuthInputs();
}

void Menu::setupRegisterMenu() {
    currentMenuType = REGISTER_MENU;
    itemCount = 3;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Register", 180, 50, NONE);
    setItem(1, "Submit", 150, 350, REGISTER);
    setItem(2, "Back", 150, 400, AUTH_SCREEN);

    finalizeBounds(1);
    resetAuthInputs();
}

void Menu::setupMainMenu() {
    currentMenuType = MAIN;
    itemCount = 8;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Play", 150, 40, START_UI);
    setItem(1, "Load Game", 150, 80, LOAD_GAME_UI);
    setItem(2, "Select Level", 150, 120, LEVEL_SELECT);
    setItem(3, "LeaderBoard", 150, 160, LEADERBOARD_UI);
    setItem(4, "Profile", 150, 200, PROFILE);
    setItem(5, "Themes", 150, 240, THEMES);
    setItem(6, "Exit", 150, 280, QUIT);

    finalizeBounds(0);
}


void Menu::setupProfileMenu() {
    currentMenuType = PROFILE;
    itemCount = 3;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "View Profile", 150, 100, PROFILE_VIEW);
    setItem(1, "Friends List", 150, 160, FRIENDS_MENU);
    //setItem(2, "Match History", 150, 220, NONE); // To be implemented
    setItem(2, "Back", 150, 280, BACK_MAIN);

    finalizeBounds(0);
}

void Menu::setupProfileView() {
    currentMenuType = PROFILE_VIEW;
    itemCount = 1;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Back", 150, 360, PROFILE);

    finalizeBounds(0);
}

void Menu::setupStartMenu() {
    currentMenuType = START_UI;
    itemCount = 3;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Single Player", 150, 150, SINGLE_PLAYER);
    //setItem(1, "Multiplayer", 150, 200, MULTIPLAYER);
    setItem(1, "Multiplayer", 150, 210, GAME_ROOM_UI);
    setItem(2, "Back", 150, 280, BACK_MAIN);

    finalizeBounds(0);
}

void Menu::setupLevelSelect() {
    currentMenuType = LEVEL_SELECT;
    itemCount = 4;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Level 1 Easy", 150, 150, EASY_LEVEL);
    setItem(1, "Level 2 Medium", 150, 220, MEDIUM_LEVEL);
    setItem(2, "Level 3 Hard", 150, 290, HARD_LEVEL);
    setItem(3, "Back", 150, 360, BACK_MAIN);

    finalizeBounds(0);
}

void Menu::setupLeaderBoard() {
    currentMenuType = LEADERBOARD_UI;
    itemCount = 2;
    selectedIndex = 0;
    clearMenuItems();

    setItem(0, "Back", 150, 300, BACK_MAIN);
    
    finalizeBounds(0);
}

void Menu::setupInventoryMenu() {
    showInventory = true;
    themeInventory->toggle();
}

void Menu::setupEndMenu() {
    currentMenuType = END;
    
    if (lastGameWasMultiplayer) {
        // Multiplayer end menu
        itemCount = 4;
        selectedIndex = 0;
        clearMenuItems();
        
        // Determine winner
        string winnerText;
        if (player1Score > player2Score) {
            winnerText = "PLAYER 1 WINS!";
        } else if (player2Score > player1Score) {
            winnerText = "PLAYER 2 WINS!";
        } else {
            winnerText = "TIE GAME!";
        }
        setItem(0, winnerText, 150, 50, NONE);
        
        setItem(1, "Restart", 150, 200, MULTIPLAYER);
        setItem(2, "Main Menu", 150, 250, BACK_MAIN);
        setItem(3, "Exit Game", 150, 280, QUIT);
        
        finalizeBounds(2);
    } else {
        // Single player end menu
        // Check if current score would make it to leaderboard
        bool isHighScore = leaderboard.shouldAdd(currentScore);
        
        itemCount = isHighScore ? 6 : 5;
        selectedIndex = 2;
        clearMenuItems();

        setItem(0, "GAME OVER", 170, 60, NONE);
        setItem(1, "Score: " + to_string(currentScore), 150, 120, NONE);
        
        if (isHighScore) {
            setItem(2, "NEW HIGH SCORE!", 130, 160, NONE);
            setItem(3, "Restart", 150, 200, SINGLE_PLAYER);
            setItem(4, "Main Menu", 150, 250, BACK_MAIN);
            setItem(5, "Exit Game", 150, 280, QUIT);
        } else {
            setItem(2, "Restart", 150, 180, SINGLE_PLAYER);
            setItem(3, "Main Menu", 150, 240, BACK_MAIN);
            setItem(4, "Exit Game", 150, 280, QUIT);
        }

        finalizeBounds(isHighScore ? 3 : 2);
    }
}

void Menu::switchTheme() {
    currentThemeIndex = (currentThemeIndex + 1) % themeTree.getSize();

    // Apply theme colors
    Theme current = getCurrentTheme();
    bgColor = current.bgColor;
    fontColor = current.textColor;
    title.setFillColor(current.titleColor);
}

void Menu::setupMultiplayerSetup() {
    currentMenuType = MULTIPLAYER;
    itemCount = 2;
    selectedIndex = 0;
    clearMenuItems();
    
    setItem(0, "Enter Player 2 Name", 120, 100, NONE);
    setItem(1, "Start Game", 150, 250, SINGLE_PLAYER);
    setItem(2, "Find Match", 150, 320, MATCHMAKING_MENU);
    setItem(3, "Back", 150, 390, BACK_MAIN);
    
    finalizeBounds(1);
    
    memset(player2Input, 0, 100);
    player2InputLen = 0;
    waitingForPlayer2 = true;
    isInputMode = true;
}

void Menu::setupFriendsMenu() {
    currentMenuType = FRIENDS_MENU;
    itemCount = 5;
    selectedIndex = 0;
    clearMenuItems();

    // Set current user for friend manager
    friendManager.setCurrentUser(string(currentPlayer.username));

    // Position menu items to leave space for content
    setItem(0, "View Friends", 10, 80, VIEW_FRIENDS);
    setItem(1, "Add Friend", 10, 120, ADD_FRIEND);
    setItem(2, "Pending Requests", 10, 160, VIEW_REQUESTS);
    setItem(3, "Sent Requests", 10, 200, VIEW_SENT_REQUESTS);
    setItem(4, "Back", 10, 240, PROFILE);

    finalizeBounds(0);

    memset(friendInput, 0, 100);
    friendInputLen = 0;
    friendError = "";
    friendSuccess = "";
    selectedRequestIndex = -1;
    showingRequests = false;
    showingSentRequests = false;
    showingFriends = true;
}

void Menu::handleFriendInput(char c) {
    if (c == '\b') { // Backspace
        if (friendInputLen > 0) {
            friendInputLen--;
            friendInput[friendInputLen] = '\0';
        }
    }
    else if (c == '\r') { // Enter
        attemptAddFriend();
    }
    else if (friendInputLen < 20 && (isalnum(c) || c == '_')) {
        friendInput[friendInputLen] = c;
        friendInputLen++;
        friendInput[friendInputLen] = '\0';
    }
}

void Menu::attemptAddFriend() {
    if (friendInputLen == 0) return;
    
    string friendName(friendInput);
    if (friendName == string(currentPlayer.username)) {
        friendError = "Cannot add yourself!";
        friendSuccess = "";
        return;
    }
    
    // Check if user exists (using AuthManager)
    if (!gameManager || !gameManager->isUsernameTaken(friendName)) {
        friendError = "User not found!";
        friendSuccess = "";
    } else if (friendManager.isFriend(friendName)) {
        friendError = "Already friends!";
        friendSuccess = "";
    } else {
        // Send friend request instead of direct add
        if (friendManager.sendFriendRequest(friendName)) {
            friendSuccess = "Friend request sent!";
            friendError = "";
        } else {
            friendError = "Request already sent!";
            friendSuccess = "";
        }
        memset(friendInput, 0, 100);
        friendInputLen = 0;
    }
}


void Menu::setupMatchmakingMenu() {
    currentMenuType = MATCHMAKING_MENU;
    itemCount = 2;
    selectedIndex = 0;
    clearMenuItems();
    
    setItem(0, "Find Match", 150, 450, FIND_MATCH);
    setItem(1, "Back", 350, 450, BACK_MAIN);
    
    finalizeBounds(0);
    
    isSearching = false;
    searchStatus = "Press 'Find Match' to start";
    
    // Add some dummy players to queue for simulation
    if (matchmakingQueue.isEmpty()) {
        matchmakingQueue.addPlayer("ProGamer123", 1500, 1, 123);
        matchmakingQueue.addPlayer("NoobMaster69", 500, 5, 155);
        matchmakingQueue.addPlayer("AverageJoe", 1000, 3, 5124);
        matchmakingQueue.addPlayer("XonixKing", 2000, 1, 513);
    }
}

void Menu::startMatchmaking() {
    isSearching = true;
    searchTimer.restart();
    searchStatus = "Searching for opponent...";
    
    // Add current player to queue
    matchmakingQueue.addPlayer(string(currentPlayer.username), 1000, 1, currentPlayer.playerID); // Assuming 1000 score for now
}

void Menu::updateMatchmaking() {
    if (!isSearching) return;
    
    float elapsed = searchTimer.getElapsedTime().asSeconds();
    
    if (elapsed > 3.0f) { // Simulate 3 seconds search
        // Find match
        QueuePlayer match = matchmakingQueue.findMatch(1000, 500); // Find match close to 1000 score
        
        if (!match.username.empty() && match.username != string(currentPlayer.username)) {
            isSearching = false;
            searchStatus = "Match Found! Opponent: " + match.username;
            
            // Set up multiplayer with this opponent
            waitingForPlayer2 = false;
            player2Name = match.username;
            player2ID = 999; // Dummy ID
            
            // Auto-start game after short delay (handled in processAction or render)
        } else {
            // Keep searching or timeout
            if (elapsed > 10.0f) {
                isSearching = false;
                searchStatus = "No match found. Try again.";
            }
        }
    }
}

//                    Input Handling 

void Menu::handleTextInput(sf::Event& e) {
    if (currentMenuType == FRIENDS_MENU) {
        if (e.type == Event::TextEntered && e.text.unicode < 128) {
            handleFriendInput(static_cast<char>(e.text.unicode));
        }
        return;
    }

    if (!isInputMode || (currentMenuType != LOGIN_MENU && currentMenuType != REGISTER_MENU && currentMenuType != MULTIPLAYER))
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
    else if (currentMenuType == MULTIPLAYER) {
        if (player2InputLen > 0) player2Input[--player2InputLen] = '\0';
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
    else if (currentMenuType == MULTIPLAYER) {
        if (player2InputLen < 99) {
            player2Input[player2InputLen++] = c;
            player2Input[player2InputLen] = '\0';
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
    return NONE;
}

UI Menu::attemptRegister() {
    username = string(usernameInput);
    password = string(passwordInput);
    string nickname = string(nicknameInput);
    string email = string(emailInput);

    if (usernameLen == 0 || passwordLen == 0) {
        registerError = "Username and password required!";
        return NONE;
    }
    else if (passwordLen < 6) {
        registerError = "Password must be 6 characters or greater";
        return NONE;
    }
    else if (gameManager->isUsernameTaken(username)) {
        registerError = "Username already taken!";
        return NONE;
    }
    else {
        if (gameManager->registerUser(username, password, nickname, email)) {
            gameManager->login(username, password);
            currentPlayer = gameManager->getCurrentPlayer();
            cout << "Registration successful! Welcome " << currentPlayer.username << endl;
            return MAIN;
        }
        else {
            registerError = "Registration failed! Password must contain atleast one letter and number";
            return NONE;
        }
    }
}

void Menu::handleKeyboard(sf::Event& e, UI& final_action) {
    if (e.type != Event::KeyPressed) return;

    // Handle inventory input first
    if (showInventory) {
        themeInventory->handleInput(e);
        if (!themeInventory->isInventoryActive()) {
            showInventory = false;
            // Apply the selected theme
            Theme selectedTheme = themeInventory->getSelectedTheme();
            if (selectedTheme.id != -1) {
                bgColor = selectedTheme.bgColor;
                fontColor = selectedTheme.textColor;
                title.setFillColor(selectedTheme.titleColor);
            }
        }
        return;
    }

    // Handle game room navigation
    if (currentMenuType == GAME_ROOM) {
        if (e.key.code == Keyboard::Up || e.key.code == Keyboard::W) {
            if (selectedPlayerIndex > 0) {
                selectedPlayerIndex--;
            }
            return;
        }
        else if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S) {
            if (selectedPlayerIndex < availablePlayers.getSize() - 1) {
                selectedPlayerIndex++;
            }
            return;
        }
        else if (e.key.code == Keyboard::Enter || e.key.code == Keyboard::Space) {
            if (availablePlayers.getSize() > 0) {
                final_action = START_GAME_WITH_SELECTED;
            }
            return;
        }
        else if (e.key.code == Keyboard::R) {
            final_action = REFRESH_PLAYERS;
            return;
        }
        else if (e.key.code == Keyboard::Escape) {
            final_action = BACK_MAIN;
            return;
        }
    }

    // Handle input mode keys
    if (currentMenuType == FRIENDS_MENU) {
        if (e.key.code == Keyboard::Enter) {
            handleFriendInput('\r'); // Simulate Enter key press for friend input
        }
        else if (e.key.code == Keyboard::Escape) {
            setupMainMenu(); // Go back to main menu from friends menu
        }
        return;
    }
    
    if (isInputMode && (currentMenuType == LOGIN_MENU || currentMenuType == REGISTER_MENU)) {
        if (e.key.code == Keyboard::Tab) {
            handleTabKey();
        }
        else if (e.key.code == Keyboard::Enter) {
            UI result = handleEnterKey();
            if (result != NONE) {
                final_action = result;
            }
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

        // Handle game room player selection
        if (currentMenuType == GAME_ROOM) {
            for (int i = 0; i < availablePlayers.getSize() && i < 8; i++) {
                // Create bounds manually for player selection
                float boundsLeft = 120.0f;
                float boundsTop = 160.0f + i * 30.0f;
                float boundsWidth = 300.0f;
                float boundsHeight = 25.0f;

                if (mouseX >= boundsLeft && mouseX <= boundsLeft + boundsWidth &&
                    mouseY >= boundsTop && mouseY <= boundsTop + boundsHeight) {
                    selectedPlayerIndex = i;
                    break;
                }
            }
        }

        // Handle regular menu items
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

        // Handle game room player selection click
        if (currentMenuType == GAME_ROOM) {
            for (int i = 0; i < availablePlayers.getSize() && i < 8; i++) {
                // Create bounds manually for player selection
                float boundsLeft = 120.0f;
                float boundsTop = 160.0f + i * 30.0f;
                float boundsWidth = 300.0f;
                float boundsHeight = 25.0f;

                if (mouseX >= boundsLeft && mouseX <= boundsLeft + boundsWidth &&
                    mouseY >= boundsTop && mouseY <= boundsTop + boundsHeight) {
                    selectedPlayerIndex = i;
                    break;
                }
            }

            // Check if click is on menu items
            for (int i = 0; i < itemCount; ++i) {
                if (menuItems[i].action != NONE && isMouseOver(menuItems[i].bounds, mouseX, mouseY)) {
                    selectedIndex = i;
                    final_action = static_cast<UI>(menuItems[selectedIndex].action);
                    break;
                }
            }
            return;
        }
        // Handle friend action buttons (Remove, Accept, Reject, Cancel)
        if (currentMenuType == FRIENDS_MENU) {
            // Handle friend removal buttons
            if (showingFriends) {
                string friendArray[MAX_FRIENDS];
                int friendCount = 0;
                friendManager.getFriendsList(friendArray, friendCount);
                for (int i = 0; i < friendCount && i < 5; i++) {
                    int yPos = 120 + i * 25;
                    // Check if click is on the Remove button area
                    if (mouseX >= 350 && mouseX <= 420 && mouseY >= yPos && mouseY <= yPos + 20) {
                        friendManager.removeFriend(friendArray[i]);
                        friendSuccess = "Friend removed: " + friendArray[i];
                        friendError = "";
                        return;
                    }
                }
            }

            // Handle request accept/reject buttons
            if (showingRequests) {
                FriendRequest pendingReqs[MAX_REQUESTS];
                int reqCount = 0;
                friendManager.getPendingRequests(pendingReqs, reqCount);
                for (int i = 0; i < reqCount && i < 4; i++) {
                    int yPos = 120 + i * 35;
                    // Accept button (350-410)
                    if (mouseX >= 350 && mouseX <= 410 && mouseY >= yPos && mouseY <= yPos + 20) {
                        if (friendManager.acceptFriendRequest(pendingReqs[i].from)) {
                            friendSuccess = "Friend request accepted!";
                            friendError = "";
                        }
                        return;
                    }
                    // Reject button (420-480)
                    if (mouseX >= 420 && mouseX <= 480 && mouseY >= yPos && mouseY <= yPos + 20) {
                        if (friendManager.rejectFriendRequest(pendingReqs[i].from)) {
                            friendSuccess = "Friend request rejected!";
                            friendError = "";
                        }
                        return;
                    }
                }
            }

            // Handle cancel sent requests
            if (showingSentRequests) {
                FriendRequest sentReqs[MAX_REQUESTS];
                int sentCount = 0;
                friendManager.getSentRequests(sentReqs, sentCount);
                for (int i = 0; i < sentCount && i < 4; i++) {
                    int yPos = 120 + i * 35;
                    // Cancel button (350-410)
                    if (mouseX >= 350 && mouseX <= 410 && mouseY >= yPos && mouseY <= yPos + 20) {
                        if (friendManager.rejectFriendRequest(sentReqs[i].to)) {
                            friendSuccess = "Friend request canceled!";
                            friendError = "";
                        }
                        return;
                    }
                }
            }
        }

        // Handle regular menu items for other menus
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
        if (currentMenuType == MULTIPLAYER) {
            player2Name = string(player2Input);
            if (player2Name.empty()) player2Name = "Player2";
            player2ID = 999; // Temporary ID for guest player
            selectedMode = MULTIPLAYER_MODE;
            return PLAY;
        }
        else {
            // Regular single player
            selectedMode = SINGLE_PLAYER_MODE;
            return PLAY;
        }
    //case MULTIPLAYER:
        //setupMultiplayerSetup();
        //break;
    case START_UI:
        setupStartMenu();
        break;
    case LOAD_GAME_UI:
        return LOAD_GAME;
    case LEVEL_SELECT:
        setupLevelSelect();
        break;
    case EASY_LEVEL:
        selectedLevel = EASY;
        setupMainMenu();
        break;
    case MEDIUM_LEVEL:
        selectedLevel = MEDIUM;
        setupMainMenu();
        break;
    case HARD_LEVEL:
        selectedLevel = HARD;
        setupMainMenu();
        break;
    case LEADERBOARD_UI:
        setupLeaderBoard();
        break;
    case PROFILE:
        setupProfileMenu();
        break;
    case PROFILE_VIEW:
        setupProfileView();
        break;
    case QUIT:
        return EXIT;
    case THEMES:
        setupInventoryMenu();
        //switchTheme();
        break;
    case FRIENDS_MENU:
        setupFriendsMenu();
        break;
    case ADD_FRIEND:
        showingFriends = false;
        showingRequests = false;
        showingSentRequests = false;
        break;
    case VIEW_FRIENDS:
        showingFriends = true;
        showingRequests = false;
        showingSentRequests = false;
        break;
    case VIEW_REQUESTS:
        showingFriends = false;
        showingRequests = true;
        showingSentRequests = false;
        break;
    case VIEW_SENT_REQUESTS:
        showingFriends = false;
        showingRequests = false;
        showingSentRequests = true;
        break;
    case REMOVE_FRIEND:
        // not really needed
        break;
    case MATCHMAKING_MENU:
        setupMatchmakingMenu();
        break;
    case GAME_ROOM_UI:
        setupGameRoom();
        break;

    case START_GAME_WITH_SELECTED:
        if (availablePlayers.getSize() > 0) {
            player2Name = availablePlayers[selectedPlayerIndex].username;
            player2ID = availablePlayers[selectedPlayerIndex].id;
            selectedMode = MULTIPLAYER_MODE;
            inGameRoom = false;
            return PLAY;
        }
        break;

    case CHANGE_TOLERANCE:
        // Cycle through tolerance levels: 200, 500, 1000, 2000
        if (matchmakingTolerance == 200) matchmakingTolerance = 500;
        else if (matchmakingTolerance == 500) matchmakingTolerance = 1000;
        else if (matchmakingTolerance == 1000) matchmakingTolerance = 2000;
        else matchmakingTolerance = 200;
        populateGameRoomWithUsers(); // Re-populate with new tolerance
        break;

    case AUTO_MATCH:
        // Use your existing matchmaking system to find the best match
        if (!gameManager || !gameManager->isLoggedIn()) break;

        {
            string currentUsername = string(currentPlayer.username);
            int currentPlayerScore = 1000;
            LeaderboardEntry sorted[LEADERBOARD_SIZE];
            leaderboard.getSortedEntries(sorted);
            for (int i = 0; i < leaderboard.getSize(); i++) {
                if (sorted[i].username == currentUsername) {
                    currentPlayerScore = sorted[i].score;
                    break;
                }
            }

            // Use matchmaking queue to find best match
            QueuePlayer bestMatch = matchmakingQueue.findMatch(currentPlayerScore, matchmakingTolerance);
            if (!bestMatch.username.empty()) {
                player2Name = bestMatch.username;
                player2ID = 999;
                selectedMode = MULTIPLAYER_MODE;
                inGameRoom = false;
                cout << "Auto-matched with: " << player2Name << " (Score: " << bestMatch.score << ")" << endl;
                return PLAY;
            }
            else {
                // No match found, refresh and select closest
                populateGameRoomWithUsers();
                if (availablePlayers.getSize() > 0) {
                    selectedPlayerIndex = 0; // Select closest match
                }
            }
        }
        break;

    case QUICK_PLAY:
        // Quick play - use wider tolerance for faster matching
        matchmakingTolerance = 2000;
        populateGameRoomWithUsers();
        if (availablePlayers.getSize() > 0) {
            selectedPlayerIndex = 0; // Select first available
            // Auto-start with closest match
            player2Name = availablePlayers[selectedPlayerIndex].username;
            player2ID = 999;
            selectedMode = MULTIPLAYER_MODE;
            inGameRoom = false;
            return PLAY;
        }
        break;

    case REFRESH_PLAYERS:
        populateGameRoomWithUsers();
        break;

    case MULTIPLAYER:
        setupGameRoom();
        break;
    case FIND_MATCH:
        startMatchmaking();
        break;
    case LOGIN_MENU:
        setupLoginMenu();
        break;
    case LOGIN:
        if (gameManager->login(username, password)) {
            currentPlayer = gameManager->getCurrentPlayer();
            isInputMode = false;
            cout << "Login successful! Welcome " << currentPlayer.username << endl;
            setupMainMenu();
        }
        else {
            loginError = "Invalid Credentials!";
        }
        break;
    case REGISTER_MENU:
        setupRegisterMenu();
        break;
    case REGISTER:
        {
            UI result = attemptRegister();
            if (result == MAIN) {
                setupMainMenu();
            }
        }
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
    // Update menu items
    for (int i = 0; i < itemCount; ++i) {
        if (i == selectedIndex) {
            menuItems[i].text.setFillColor(getCurrentTheme().highlightColor);
        } else {
            menuItems[i].text.setFillColor(fontColor);
        }
    }
}

void Menu::update() {
    updateColors();
    updateMatchmaking();
}

void Menu::renderLoginScreen() {
    Text title("Login", font, fontSize);
    title.setPosition(200, 30);
    title.setFillColor(fontColor);
    window->draw(title);

    Text userLabel("Username: " + string(usernameInput), font, 30);
    userLabel.setPosition(100, 120);
    userLabel.setFillColor(currentInputField == 0 ? getCurrentTheme().highlightColor : fontColor);
    window->draw(userLabel);

    string passDisplay(passwordLen, '*');
    Text passLabel("Password: " + passDisplay, font, 30);
    passLabel.setPosition(100, 160);
    passLabel.setFillColor(currentInputField == 1 ? getCurrentTheme().highlightColor : fontColor);
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
    userLabel.setFillColor(currentInputField == 0 ? getCurrentTheme().highlightColor : fontColor);
    window->draw(userLabel);

    string passDisplay(passwordLen, '*');
    Text passLabel("Password: " + passDisplay, font, 25);
    passLabel.setPosition(80, 140);
    passLabel.setFillColor(currentInputField == 1 ? getCurrentTheme().highlightColor : fontColor);
    window->draw(passLabel);

    Text nickLabel("Nickname (optional): " + string(nicknameInput), font, 25);
    nickLabel.setPosition(80, 180);
    nickLabel.setFillColor(currentInputField == 2 ? getCurrentTheme().highlightColor : fontColor);
    window->draw(nickLabel);

    Text emailLabel("Email (optional): " + string(emailInput), font, 25);
    emailLabel.setPosition(80, 220);
    emailLabel.setFillColor(currentInputField == 3 ? getCurrentTheme().highlightColor : fontColor);
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

    for (int i = 1; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
    }
}

void Menu::renderAuthScreen() {
    Text title("Welcome to Xonix Game", font, fontSize);
    title.setPosition(100, 50);
    title.setFillColor(fontColor);
    window->draw(title);

    for (int i = 0; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
    }
}

void Menu::renderProfileMenu() {
    Text title("Player Profile", font, fontSize);
    title.setPosition(150, 30);
    title.setFillColor(fontColor);
    window->draw(title);

    // Draw menu items
    for (int i = 0; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
    }
}

void Menu::renderProfileView() {
    Text title("Player Profile - " + string(currentPlayer.nickname), font, fontSize);
    title.setPosition(100, 30);
    title.setFillColor(fontColor);
    window->draw(title);

    // Player information
    Text userInfo("Username: " + string(currentPlayer.username), font, 25);
    userInfo.setPosition(100, 80);
    userInfo.setFillColor(fontColor);
    window->draw(userInfo);

    Text nickInfo("Nickname: " + string(currentPlayer.nickname), font, 25);
    nickInfo.setPosition(100, 110);
    nickInfo.setFillColor(fontColor);
    window->draw(nickInfo);

    if (currentPlayer.email[0] != '\0') {
        Text emailInfo("Email: " + string(currentPlayer.email), font, 25);
        emailInfo.setPosition(100, 140);
        emailInfo.setFillColor(fontColor);
        window->draw(emailInfo);
    }

    Text regInfo("Registered: " + string(currentPlayer.registrationDate), font, 25);
    regInfo.setPosition(100, 170);
    regInfo.setFillColor(fontColor);
    window->draw(regInfo);

    Text playerIDInfo("Player ID: " + to_string(currentPlayer.playerID), font, 25);
    playerIDInfo.setPosition(100, 200);
    playerIDInfo.setFillColor(fontColor);
    window->draw(playerIDInfo);

    // Game statistics 
    Text statsTitle("Game Statistics", font, 28);
    statsTitle.setPosition(100, 240);
    statsTitle.setFillColor(Color::Cyan);
    window->draw(statsTitle);

    // These would come from your game data storage
    Text totalGames("Total Games: " + to_string(currentPlayer.totalGames), font, 22);
    totalGames.setPosition(120, 280);
    totalGames.setFillColor(fontColor);
    window->draw(totalGames);

    Text totalPoints("Total Points: " + to_string(currentPlayer.totalPoints), font, 22);
    totalPoints.setPosition(120, 310);
    totalPoints.setFillColor(fontColor);
    window->draw(totalPoints);

    // Draw back buttons
    for (int i = 0; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
    }
}

void Menu::renderLeaderBoard() {
    Text title("LEADERBOARD - TOP 10", font, fontSize);
    title.setPosition(80, 30);
    title.setFillColor(Color::Yellow);
    window->draw(title);

    LeaderboardEntry sorted[LEADERBOARD_SIZE];
    leaderboard.getSortedEntries(sorted);
    int leaderboardSize = leaderboard.getSize();

    // Display column headers
    Text rankHeader("RANK", font, 20);
    rankHeader.setPosition(80, 80);
    rankHeader.setFillColor(getCurrentTheme().titleColor);
    window->draw(rankHeader);

    Text nameHeader("PLAYER NAME", font, 20);
    nameHeader.setPosition(150, 80);
    nameHeader.setFillColor(getCurrentTheme().titleColor);
    window->draw(nameHeader);

    Text idHeader("PLAYER ID", font, 20);
    idHeader.setPosition(300, 80);
    idHeader.setFillColor(getCurrentTheme().titleColor);
    window->draw(idHeader);

    Text scoreHeader("SCORE", font, 20);
    scoreHeader.setPosition(380, 80);
    scoreHeader.setFillColor(getCurrentTheme().titleColor);
    window->draw(scoreHeader);

    // Display leaderboard entries
    for (int i = 0; i < leaderboardSize && i < 10; i++) {
        // Rank
        Text rankText(to_string(i + 1) + ".", font, 18);
        rankText.setPosition(80, 110 + i * 25);
        rankText.setFillColor(fontColor);
        window->draw(rankText);

        // Player name
        Text nameText(sorted[i].username, font, 18);
        nameText.setPosition(120, 110 + i * 25);
        nameText.setFillColor(fontColor);
        window->draw(nameText);

        // Player ID
        Text idText(to_string(sorted[i].playerID), font, 18);
        idText.setPosition(300, 110 + i * 25);
        idText.setFillColor(fontColor);
        window->draw(idText);

        // Score
        Text scoreText(to_string(sorted[i].score), font, 18);
        scoreText.setPosition(380, 110 + i * 25);
        scoreText.setFillColor(fontColor);
        window->draw(scoreText);
    }

    if (leaderboardSize == 0) {
        Text noEntries("No entries yet! Play some games!", font, 25);
        noEntries.setPosition(80, 150);
        noEntries.setFillColor(Color::Red);
        window->draw(noEntries);
    }

    // Instructions
    Text instruction("Press BACK to return to main menu", font, 16);
    instruction.setPosition(80, 400);
    instruction.setFillColor(Color::Green);
    window->draw(instruction);

    // Draw back button
    for (int i = 0; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
    }
}

void Menu::renderNormalMenu() {
    // If it's the end menu and was multiplayer, show scores
    if (currentMenuType == END && lastGameWasMultiplayer) {
        // Draw menu items first
        for (int i = 0; i < itemCount; ++i) {
            window->draw(menuItems[i].text);
        }
        
        // Draw player scores below the winner text
        Text p1ScoreText("Player 1: " + to_string(player1Score), font, 30);
        p1ScoreText.setPosition(150, 120);
        p1ScoreText.setFillColor(Color::Red);
        window->draw(p1ScoreText);
        
        Text p2ScoreText("Player 2: " + to_string(player2Score), font, 30);
        p2ScoreText.setPosition(150, 160);
        p2ScoreText.setFillColor(Color::Blue);
        window->draw(p2ScoreText);
    } else {
        // Normal menu rendering
        for (int i = 0; i < itemCount; ++i) {
            window->draw(menuItems[i].text);
        }
    }
}

void Menu::renderMultiplayerSetup() {
    Text title("Multiplayer Setup", font, fontSize);
    title.setPosition(120, 30);
    title.setFillColor(fontColor);
    window->draw(title);
    
    Text p1Info("Player 1: " + getCurrentUsername(), font, 25);
    p1Info.setPosition(100, 100);
    p1Info.setFillColor(Color::Green);
    window->draw(p1Info);
    
    Text p2Label("Player 2 Name: " + string(player2Input), font, 25);
    p2Label.setPosition(100, 150);
    p2Label.setFillColor(getCurrentTheme().highlightColor);
    window->draw(p2Label);
    
    Text instruction("Enter name and press START GAME", font, 20);
    instruction.setPosition(80, 200);
    instruction.setFillColor(fontColor);
    window->draw(instruction);
    
    for (int i = 1; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
 
   }
}

void Menu::renderFriendsMenu() {
    Text title("Friends Management", font, fontSize);
    title.setPosition(200, 30);
    title.setFillColor(Color(255, 140, 0)); // Orange
    window->draw(title);

    for (int i = 0; i < itemCount; ++i) {
        window->draw(menuItems[i].text);
    }

    // Display different sections based on what's selected AFTER drawing menu items
    if (showingFriends) {
        renderFriendsList();
    }
    else if (showingRequests) {
        renderPendingRequests();
    }
    else if (showingSentRequests) {
        renderSentRequests();
    }
    else {
        // This handles the case when "Add Friend" menu item is selected
        renderAddFriendSection();
    }

}

void Menu::renderFriendsList() {
    Text sectionTitle("Your Friends", font, 24);
    sectionTitle.setPosition(240, 80);
    sectionTitle.setFillColor(Color(255, 140, 0)); // Orange
    window->draw(sectionTitle);

    // Display friends
    string friendArray[MAX_FRIENDS];
    int friendCount = 0;
    friendManager.getFriendsList(friendArray, friendCount);

    if (friendCount == 0) {
        Text noFriends("No friends yet!", font, 18);
        noFriends.setPosition(240, 120);
        noFriends.setFillColor(Color(169, 169, 169)); // Dark gray
        window->draw(noFriends);

        Text addHint("Use 'Add Friend' to find friends", font, 14);
        addHint.setPosition(240, 150);
        addHint.setFillColor(Color(128, 128, 128));
        window->draw(addHint);
    }
    else {
        for (int i = 0; i < friendCount && i < 5; i++) {
            Text friendText;
            friendText.setFont(font);
            friendText.setString(to_string(i + 1) + ". " + friendArray[i]);
            friendText.setCharacterSize(16);
            friendText.setPosition(200, 120 + i * 25);
            friendText.setFillColor(Color(220, 220, 220)); // Light gray
            window->draw(friendText);

            // Remove friend button - Orange
            Text removeBtn;
            removeBtn.setFont(font);
            removeBtn.setString("[Remove]");
            removeBtn.setCharacterSize(12);
            removeBtn.setPosition(400, 120 + i * 25);
            removeBtn.setFillColor(Color(255, 140, 0)); // Orange
            window->draw(removeBtn);
        }

        Text instruction("Click [Remove] to remove a friend", font, 12);
        instruction.setPosition(300, 120 + min(friendCount, 5) * 25 + 5);
        instruction.setFillColor(Color(255, 165, 0)); // Bright orange
        window->draw(instruction);
    }
}

void Menu::renderPendingRequests() {
    Text sectionTitle("Pending Requests", font, 24);
    sectionTitle.setPosition(300, 80);
    sectionTitle.setFillColor(Color(255, 140, 0)); // Orange
    window->draw(sectionTitle);

    // Display pending friend requests
    FriendRequest pendingReqs[MAX_REQUESTS];
    int reqCount = 0;
    friendManager.getPendingRequests(pendingReqs, reqCount);

    if (reqCount == 0) {
        Text noReqs("No pending requests", font, 18);
        noReqs.setPosition(300, 120);
        noReqs.setFillColor(Color(169, 169, 169)); // Dark gray
        window->draw(noReqs);
    }
    else {
        for (int i = 0; i < reqCount && i < 4; i++) {
            // Request info
            Text reqText;
            reqText.setFont(font);
            reqText.setString(pendingReqs[i].from);
            reqText.setCharacterSize(16);
            reqText.setPosition(300, 120 + i * 35);
            reqText.setFillColor(Color(220, 220, 220)); // Light gray
            window->draw(reqText);

            // Accept button - Green
            Text acceptBtn;
            acceptBtn.setFont(font);
            acceptBtn.setString("[Accept]");
            acceptBtn.setCharacterSize(12);
            acceptBtn.setPosition(400, 120 + i * 35);
            acceptBtn.setFillColor(Color(0, 255, 0)); // Green
            window->draw(acceptBtn);

            // Reject button - Orange-red
            Text rejectBtn;
            rejectBtn.setFont(font);
            rejectBtn.setString("[Reject]");
            rejectBtn.setCharacterSize(12);
            rejectBtn.setPosition(420, 120 + i * 35);
            rejectBtn.setFillColor(Color(255, 69, 0)); // Red-orange
            window->draw(rejectBtn);
        }
    }
}

void Menu::renderSentRequests() {
    Text sectionTitle("Sent Requests", font, 24);
    sectionTitle.setPosition(300, 80);
    sectionTitle.setFillColor(Color(255, 140, 0)); // Orange
    window->draw(sectionTitle);

    // Display sent friend requests
    FriendRequest sentReqs[MAX_REQUESTS];
    int sentCount = 0;
    friendManager.getSentRequests(sentReqs, sentCount);

    if (sentCount == 0) {
        Text noSent("No sent requests", font, 18);
        noSent.setPosition(300, 120);
        noSent.setFillColor(Color(169, 169, 169)); // Dark gray
        window->draw(noSent);
    }
    else {
        for (int i = 0; i < sentCount && i < 4; i++) {
            Text reqText;
            reqText.setFont(font);
            reqText.setString(sentReqs[i].to);
            reqText.setCharacterSize(16);
            reqText.setPosition(300, 120 + i * 35);
            reqText.setFillColor(Color(220, 220, 220)); // Light gray
            window->draw(reqText);

            // Cancel request button - Orange
            Text cancelBtn;
            cancelBtn.setFont(font);
            cancelBtn.setString("[Cancel]");
            cancelBtn.setCharacterSize(12);
            cancelBtn.setPosition(350, 120 + i * 35);
            cancelBtn.setFillColor(Color(255, 140, 0)); // Orange
            window->draw(cancelBtn);
        }
    }
}

void Menu::renderAddFriendSection() {
    Text sectionTitle("Add Friend", font, 24);
    sectionTitle.setPosition(300, 80);
    sectionTitle.setFillColor(Color(255, 140, 0)); // Orange
    window->draw(sectionTitle);

    // Instructions
    Text instructions("Enter username:", font, 16);
    instructions.setPosition(300, 120);
    instructions.setFillColor(Color(255, 140, 0)); // Orange
    window->draw(instructions);

    // Add friend input
    Text addLabel("> " + string(friendInput), font, 18);
    addLabel.setPosition(300, 150);
    addLabel.setFillColor(getCurrentTheme().highlightColor);
    window->draw(addLabel);

    if (!friendError.empty()) {
        Text error(friendError, font, 14);
        error.setPosition(300, 180);
        error.setFillColor(Color::Red);
        window->draw(error);
    }

    if (!friendSuccess.empty()) {
        Text success(friendSuccess, font, 14);
        success.setPosition(300, 180);
        success.setFillColor(Color::Green);
        window->draw(success);
    }

    // Friend count display
    int friendCount = friendManager.getFriendsCount();
    Text countText("Friends: " + to_string(friendCount), font, 14);
    countText.setPosition(300, 210);
    countText.setFillColor(Color(255, 165, 0)); // Orange
    window->draw(countText);
}

void Menu::populateGameRoomWithUsers() {
    availablePlayers.clear();

    if (!gameManager || !gameManager->isLoggedIn()) {
        return;
    }

    string currentUsername = string(currentPlayer.username);

    // Get current player's score once
    int currentPlayerScore = getCurrentPlayerScore();

    // Get leaderboard data once
    LeaderboardEntry sorted[LEADERBOARD_SIZE];
    leaderboard.getSortedEntries(sorted);
    int leaderboardSize = leaderboard.getSize();

    Vector<QueuePlayer> allEligiblePlayers;

    // Get all eligible users in one pass
    for (int i = 0; i < gameManager->getUserCount(); i++) {
        Player user = gameManager->getUserByIndex(i);
        string username = string(user.username);

        // Skip the logged-in user and empty usernames
        if (username == currentUsername || username.length() == 0) {
            continue;
        }

        // Get user's score and rank
        int userScore = 0;
        int userRank = 999;

        for (int j = 0; j < leaderboardSize; j++) {
            if (sorted[j].username == username) {
                userScore = sorted[j].score;
                userRank = j + 1;
                break;
            }
        }

        // Calculate score difference
        int scoreDiff = abs(currentPlayerScore - userScore);
        QueuePlayer player(username, userScore, userRank, user.playerID);

        // Separate logic for within tolerance vs all players
        if (scoreDiff <= matchmakingTolerance) {
            allEligiblePlayers.push_back(player);
        }
    }

    // Sort by score proximity
    sortPlayersByProximity(allEligiblePlayers, currentPlayerScore);

    // If no players within tolerance, get all players and sort them
    if (allEligiblePlayers.getSize() == 0) {
        for (int i = 0; i < gameManager->getUserCount(); i++) {
            Player user = gameManager->getUserByIndex(i);
            string username = string(user.username);

            if (username != currentUsername && username.length() > 0) {
                int userScore = 0;
                int userRank = 999;

                for (int j = 0; j < leaderboardSize; j++) {
                    if (sorted[j].username == username) {
                        userScore = sorted[j].score;
                        userRank = j + 1;
                        break;
                    }
                }
                allEligiblePlayers.push_back(QueuePlayer(username, userScore, userRank, user.playerID));
            }
        }
        sortPlayersByProximity(allEligiblePlayers, currentPlayerScore);
    }

    // Limit to top 8 matches
    availablePlayers.clear();
    int limit = min(allEligiblePlayers.getSize(), 8);
    for (int i = 0; i < limit; i++) {
        availablePlayers.push_back(allEligiblePlayers[i]);
    }

    selectedPlayerIndex = 0;
}

int Menu::getCurrentPlayerScore() {
    string currentUsername = string(currentPlayer.username);
    LeaderboardEntry sorted[LEADERBOARD_SIZE];
    leaderboard.getSortedEntries(sorted);

    for (int i = 0; i < leaderboard.getSize(); i++) {
        if (sorted[i].username == currentUsername) {
            return sorted[i].score;
        }
    }
    return 1000; // Default score if not found
}

void Menu::sortPlayersByProximity(Vector<QueuePlayer>& players, int targetScore) {
    int size = players.getSize();
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            int diff1 = abs(players[j].score - targetScore);
            int diff2 = abs(players[j + 1].score - targetScore);
            if (diff1 > diff2) {
                QueuePlayer temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }
}

void Menu::setupGameRoom() {
    currentMenuType = GAME_ROOM;
    itemCount = 4;
    selectedIndex = 0;
    clearMenuItems();

    populateGameRoomWithUsers();

    setItem(0, "Select Opponent", 150, 50, NONE);
    setItem(1, "Start Game", 150, 350, START_GAME_WITH_SELECTED);
    setItem(2, "Refresh Players", 150, 400, REFRESH_PLAYERS);
    setItem(3, "Back", 150, 450, BACK_MAIN);

    finalizeBounds(1);
    inGameRoom = true;
}

void Menu::renderGameRoom() {
    Text title("Game Room", font, fontSize);
    title.setPosition(80, 30);
    title.setFillColor(fontColor);
    window->draw(title);

    // Display current player info with score
    string currentUsername = string(currentPlayer.username);
    int currentPlayerScore = 1000;
    LeaderboardEntry sorted[LEADERBOARD_SIZE];
    leaderboard.getSortedEntries(sorted);
    for (int i = 0; i < leaderboard.getSize(); i++) {
        if (sorted[i].username == currentUsername) {
            currentPlayerScore = sorted[i].score;
            break;
        }
    }

    Text currentPlayerText("You: " + currentUsername + " (Score: " + to_string(currentPlayerScore) + ")", font, 25);
    currentPlayerText.setPosition(100, 70);
    currentPlayerText.setFillColor(Color::Green);
    window->draw(currentPlayerText);

    // Display matchmaking tolerance
    Text toleranceText("Matchmaking Tolerance: ±" + to_string(matchmakingTolerance) + " points", font, 20);
    toleranceText.setPosition(100, 100);
    toleranceText.setFillColor(Color::Yellow);
    window->draw(toleranceText);

    // Display available players with scores
    Text availableTitle("Available Players (sorted by skill proximity):", font, 22);
    availableTitle.setPosition(100, 130);
    availableTitle.setFillColor(getCurrentTheme().titleColor);
    window->draw(availableTitle);

    if (availablePlayers.getSize() == 0) {
        Text noPlayers("No suitable players found within tolerance", font, 18);
        noPlayers.setPosition(120, 160);
        noPlayers.setFillColor(Color::Red);
        window->draw(noPlayers);

        Text expandHint("Try increasing tolerance or refreshing", font, 16);
        expandHint.setPosition(120, 190);
        expandHint.setFillColor(Color::Yellow);
        window->draw(expandHint);
    }
    else {
        for (int i = 0; i < availablePlayers.getSize() && i < 8; i++) {
            Text playerText;
            playerText.setFont(font);

            // Calculate score difference
            int scoreDiff = availablePlayers[i].score - currentPlayerScore;
            string diffStr = (scoreDiff >= 0) ? "+" + to_string(scoreDiff) : to_string(scoreDiff);

            playerText.setString(to_string(i + 1) + ". " + availablePlayers[i].username +
                " (Score: " + to_string(availablePlayers[i].score) +
                ", Diff: " + diffStr + ")");
            playerText.setCharacterSize(18);
            playerText.setPosition(120, 160 + i * 25);

            // Color coding based on score difference
            if (abs(scoreDiff) <= 100) {
                playerText.setFillColor(Color::Green); // Very close match
            }
            else if (abs(scoreDiff) <= 300) {
                playerText.setFillColor(Color::Yellow); // Good match
            }
            else {
                playerText.setFillColor(Color(255, 165, 0)); // Orange - fair match
            }

            if (i == selectedPlayerIndex) {
                playerText.setFillColor(getCurrentTheme().highlightColor);
                string selectedText = "> " + playerText.getString();
                playerText.setString(selectedText);
            }

            window->draw(playerText);
        }

        // Show selected player info with match quality
        if (availablePlayers.getSize() > 0) {
            int selectedScoreDiff = availablePlayers[selectedPlayerIndex].score - currentPlayerScore;
            string matchQuality;
            Color qualityColor;

            if (abs(selectedScoreDiff) <= 100) {
                matchQuality = "Excellent Match!";
                qualityColor = Color::Green;
            }
            else if (abs(selectedScoreDiff) <= 300) {
                matchQuality = "Good Match";
                qualityColor = Color::Yellow;
            }
            else if (abs(selectedScoreDiff) <= matchmakingTolerance) {
                matchQuality = "Fair Match";
                qualityColor = Color(255, 165, 0);
            }
            else {
                matchQuality = "Skill Gap";
                qualityColor = Color::Red;
            }

            Text selectedInfo("Selected: " + availablePlayers[selectedPlayerIndex].username +
                " | " + matchQuality, font, 20);
            selectedInfo.setPosition(100, 380);
            selectedInfo.setFillColor(qualityColor);
            window->draw(selectedInfo);
        }
    }

    // Draw menu items
    for (int i = 1; i < itemCount; i++) {
        window->draw(menuItems[i].text);
    }
}

void Menu::renderMatchmakingMenu() {
    return;
}

void Menu::render() {
    window->clear(bgColor);

    // Render inventory if active
    if (showInventory) {
        themeInventory->render(window);
        window->display();
        return;
    }

    if (currentMenuType == LOGIN_MENU) {
        renderLoginScreen();
    }
    else if (currentMenuType == REGISTER_MENU) {
        renderRegisterScreen();
    }
    else if (currentMenuType == FRIENDS_MENU) {
        renderFriendsMenu();
    }
    else if (currentMenuType == MATCHMAKING_MENU) {
        renderMatchmakingMenu();
    }
    else if (currentMenuType == AUTH_SCREEN) {
        renderAuthScreen();
    }
    else if (currentMenuType == LEADERBOARD_UI) {
        renderLeaderBoard();
    }
    else if (currentMenuType == MULTIPLAYER) {
        renderMultiplayerSetup();
    }
    else if (currentMenuType == PROFILE) {
        renderProfileMenu();
    }
    else if (currentMenuType == PROFILE_VIEW) {
        renderProfileView();
    }
    else if (currentMenuType == GAME_ROOM) {
        renderGameRoom();
    }
    else {
        renderNormalMenu();
    }

    window->display();
}

//          Main Menu Loop 

MenuOptions Menu::runMainMenu(bool m) {
    if (!font.loadFromFile("assets/monogram.ttf")) {
        return EXIT;
    }

    if (!gameManager || !gameManager->isLoggedIn() && m) {
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