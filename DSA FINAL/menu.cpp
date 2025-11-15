#include "menu.hpp"

Menu::Menu(RenderWindow* window) : window(window), currentScore(0) {}

void Menu::setupMainMenu() {

    currentMenuType = MAIN;
    itemCount = 5;
    selectedIndex = 0; // Default selection

    // Clear previous items and positions
    for (int i = 0; i < MAX_MENU_ITEMS; ++i) {
        menuItems[i].text.setString("");
    }

    // 1. Play
    menuItems[0].text.setFont(font);
    menuItems[0].text.setString("Play");
    menuItems[0].text.setCharacterSize(fontSize);
    menuItems[0].text.setPosition(150, 80);
    menuItems[0].action = START_UI;
    
    // 2. Select Level
    menuItems[1].text.setFont(font);
    menuItems[1].text.setString("Select Level");
    menuItems[1].text.setCharacterSize(fontSize);
    menuItems[1].text.setPosition(150, 150);
    menuItems[1].action = LEVEL_SELECT;

    menuItems[2].text.setFont(font);
    menuItems[2].text.setString("LeaderBoard");
    menuItems[2].text.setCharacterSize(fontSize);
    menuItems[2].text.setPosition(150, 220);
    menuItems[2].action = LEADERBOARD;

    // 4. Themes
    menuItems[3].text.setFont(font);
    menuItems[3].text.setString("Themes");
    menuItems[3].text.setCharacterSize(fontSize);
    menuItems[3].text.setPosition(150, 290);
    menuItems[3].action = THEMES;

    // 5. Exit
    menuItems[4].text.setFont(font);
    menuItems[4].text.setString("Exit");
    menuItems[4].text.setCharacterSize(fontSize);
    menuItems[4].text.setPosition(150, 360);
    menuItems[4].action = QUIT;

    // Calculate bounds for mouse interaction (do this once per setup)
    for (int i = 0; i < itemCount; ++i) {
        menuItems[i].bounds = menuItems[i].text.getGlobalBounds();
    }
}

void Menu::setupStartMenu() {

    currentMenuType = START_UI;
    itemCount = 3;
    selectedIndex = 0; // Default selection

    // Clear previous items and positions
    for (int i = 0; i < MAX_MENU_ITEMS; ++i) {
        menuItems[i].text.setString("");
    }

    // 1. Play
    menuItems[0].text.setFont(font);
    menuItems[0].text.setString("SinglePlayer");
    menuItems[0].text.setCharacterSize(fontSize);
    menuItems[0].text.setPosition(150, 150);
    menuItems[0].action = SINGLE_PLAYER;

    // 2. Select Level
    menuItems[1].text.setFont(font);
    menuItems[1].text.setString("MultiPlayer");
    menuItems[1].text.setCharacterSize(fontSize);
    menuItems[1].text.setPosition(150, 220);
    menuItems[1].action = MULTIPLAYER;

    menuItems[2].text.setFont(font);
    menuItems[2].text.setString("Back");
    menuItems[2].text.setCharacterSize(fontSize);
    menuItems[2].text.setPosition(150, 290);
    menuItems[2].action = BACK_MAIN;

    // Calculate bounds for mouse interaction (do this once per setup)
    for (int i = 0; i < itemCount; ++i) {
        menuItems[i].bounds = menuItems[i].text.getGlobalBounds();
    }
}

void Menu::setupLevelSelect() {

    currentMenuType = LEVEL_SELECT;
    itemCount = 4;
    selectedIndex = 0; // Default selection

    // Clear previous items and positions
    for (int i = 0; i < MAX_MENU_ITEMS; ++i) {
        menuItems[i].text.setString("");
    }

    // 1. Level 1
    menuItems[0].text.setFont(font);
    menuItems[0].text.setString("Level 1 Easy");
    menuItems[0].text.setCharacterSize(fontSize);
    menuItems[0].text.setPosition(150, 150);
    menuItems[0].action = EASY;

    // 2. Level 2
    menuItems[1].text.setFont(font);
    menuItems[1].text.setString("Level 2 Medium");
    menuItems[1].text.setCharacterSize(fontSize);
    menuItems[1].text.setPosition(150, 220);
    menuItems[1].action = MEDIUM;

    // 3. Level 3
    menuItems[2].text.setFont(font);
    menuItems[2].text.setString("Level 3 Hard");
    menuItems[2].text.setCharacterSize(fontSize);
    menuItems[2].text.setPosition(150, 290);
    menuItems[2].action = HARD;

    // 3. Back
    menuItems[3].text.setFont(font);
    menuItems[3].text.setString("Back");
    menuItems[3].text.setCharacterSize(fontSize);
    menuItems[3].text.setPosition(150, 360);
    menuItems[3].action = BACK_MAIN;

    // Calculate bounds for mouse interaction (do this once per setup)
    for (int i = 0; i < itemCount; ++i) {
        menuItems[i].bounds = menuItems[i].text.getGlobalBounds();
    }
}

void Menu::setupLeaderBoard() {
    // Will fill this later not now I guess
}

void Menu::setupEndMenu() {

    currentMenuType = END;
    itemCount = 5;
    selectedIndex = 2; // Default selection

    // Clear previous items and positions
    for (int i = 0; i < MAX_MENU_ITEMS; ++i) {
        menuItems[i].text.setString("");
    }

    // 1. Game Over (non-clickable header)
    menuItems[0].text.setFont(font);
    menuItems[0].text.setString("Game Over");
    menuItems[0].text.setCharacterSize(fontSize);
    menuItems[0].text.setPosition(170, 80);
    menuItems[0].action = NONE; // Explicitly set to NONE since it's not clickable

    // 2. Score (non-clickable display)
    menuItems[1].text.setFont(font);
    menuItems[1].text.setString("Score: " + std::to_string(currentScore));
    menuItems[1].text.setCharacterSize(fontSize);
    menuItems[1].text.setPosition(150, 150);
    menuItems[1].action = NONE; // Explicitly set to NONE since it's not clickable

    menuItems[2].text.setFont(font);
    menuItems[2].text.setString("Restart");
    menuItems[2].text.setCharacterSize(fontSize);
    menuItems[2].text.setPosition(150, 220);
    menuItems[2].action = SINGLE_PLAYER;

    // 4. Main Menu
    menuItems[3].text.setFont(font);
    menuItems[3].text.setString("Main Menu");
    menuItems[3].text.setCharacterSize(fontSize);
    menuItems[3].text.setPosition(150, 290);
    menuItems[3].action = BACK_MAIN; // Change from MAIN to BACK_MAIN

    menuItems[4].text.setFont(font);
    menuItems[4].text.setString("Exit Game");
    menuItems[4].text.setCharacterSize(fontSize);
    menuItems[4].text.setPosition(150, 360);
    menuItems[4].action = QUIT;

    // Calculate bounds for mouse interaction (only for clickable items)
    for (int i = 2; i < itemCount; ++i) {
        menuItems[i].bounds = menuItems[i].text.getGlobalBounds();
    }
}

void Menu::switchTheme() {
    // Cycle through Themes
    theme = (theme + 1) % 8;
    if (theme == 0) {
        bgColor = { 0, 0, 0 };
        fontColor = { 255,255,255 };
    }
    else if (theme == 1) {
        bgColor = { 255,255,255 };
        fontColor = { 0,0,0 };
    }
    else if (theme == 2) bgColor = { 0,0,255 };
    else if (theme == 3) bgColor = { 255, 0, 0 };
    else if (theme == 4) bgColor = { 255, 0, 255 };
    else if (theme == 5) bgColor = { 0, 0, 155 };
    else if (theme == 6) bgColor = { 0, 155, 0 };
    else if (theme == 7) bgColor = { 155, 0, 0 };
}

bool Menu::isMouseOver(const sf::FloatRect& bounds, int mouseX, int mouseY) const {
    // The FloatRect::contains method checks if the point (mouseX, mouseY) falls within the bounds rectangle.
    return bounds.contains(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

void Menu::setScore(int score) {
    currentScore = score;
}

MenuOptions Menu::runMainMenu(bool m) {
   
    if (!font.loadFromFile("assets/monogram.ttf")) {
        // fallback or handle error
    }

    // INITIALIZATION 
    if (m == true) {
        setupMainMenu();
    }
    else {
        setupEndMenu();
    }

    // EVENT HANDLER

    while (window->isOpen()) {

        UI final_action = MAIN;

        Event e;
        while (window->pollEvent(e)) {
            if (e.type == Event::Closed)
                window->close();

            // --- KEYBOARD INPUT HANDLING ---
            if (e.type == Event::KeyPressed) {

                if (e.key.code == Keyboard::Up || e.key.code == Keyboard::W) {
                    do {
                        selectedIndex = (selectedIndex + itemCount - 1) % itemCount;
                    } while (menuItems[selectedIndex].action == NONE && itemCount > 1);
                }
                if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S) {
                    do {
                        selectedIndex = (selectedIndex + 1) % itemCount;
                    } while (menuItems[selectedIndex].action == NONE && itemCount > 1);
                }

                if (e.key.code == Keyboard::Enter || e.key.code == Keyboard::Space) {
                    // Return the action mapped to the currently selected item
                    if (menuItems[selectedIndex].action != NONE) {
                        final_action = static_cast<UI>(menuItems[selectedIndex].action);
                    }
                }
            }

            // MOUSE SELECTION
            if (e.type == Event::MouseMoved) {
                int mouseX = e.mouseMove.x;
                int mouseY = e.mouseMove.y;

                // Check for hover and update selection (skip non-clickable items)
                for (int i = 0; i < itemCount; ++i) {
                    if (menuItems[i].action != NONE && isMouseOver(menuItems[i].bounds, mouseX, mouseY)) {
                        selectedIndex = i;
                        break;
                    }
                }
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                int mouseX = e.mouseButton.x;
                int mouseY = e.mouseButton.y;

                // Check for click and return action (skip non-clickable items)
                for (int i = 0; i < itemCount; ++i) {
                    if (menuItems[i].action != NONE && isMouseOver(menuItems[i].bounds, mouseX, mouseY)) {
                        selectedIndex = i;
                        final_action = static_cast<UI>(menuItems[selectedIndex].action);
                    }
                }
            }

        }

        // Highlight selection and theming
        for (int i = 0; i < itemCount; ++i) {

            if (i == selectedIndex) {
                menuItems[i].text.setFillColor(Color::Yellow);
            }
            else 
                menuItems[i].text.setFillColor(fontColor);
        }

        // TRANSITIONS/UI LOGIC
        if (final_action != MAIN) {
            switch (final_action) {
            case SINGLE_PLAYER: // This is triggered by "Restart" in end menu or selecting a level
                return PLAY; // Exit the menu system and start the game!
            case UI::LEVEL_SELECT:
                setupLevelSelect();
                break;
            case UI::LEADERBOARD:
                setupLeaderBoard();
                break;
            case UI::QUIT:
                return EXIT; // Exit the entire game
            case UI::START_UI: // START is triggered by "Play" in the main menu
                setupStartMenu();
                break;
            case UI::THEMES:
                switchTheme();
                break;
            case UI::BACK_MAIN: // When "Main Menu" is selected from end menu, or "Back" from sub-menus
                if (currentMenuType == END) {
                    return MENU; // Exit end menu and return to main menu in main loop
                } else {
                    setupMainMenu(); // For other menus, just switch internally
                }
                break;
            case UI::END:
                setupEndMenu();
                break;
            default:
                break;
            }
        }

        // draw menu
        window->clear(bgColor);
        window->draw(title);
        for (int i = 0; i < itemCount; ++i) {
            window->draw(menuItems[i].text);
        }
        window->display();  
    }

    return EXIT; // if window is closed
}