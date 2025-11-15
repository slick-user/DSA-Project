#include "menu.hpp"

Menu::Menu(RenderWindow* window) : window(window) {}

void Menu::setupMainMenu() {

    if (!font.loadFromFile("assets/monogram.ttf")) {
        // fallback or handle error
    }

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

MenuOptions Menu::runMainMenu() {
   
    // INITIALIZATION 

    setupMainMenu();

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
                    // Use itemCount (which is 4) for modular arithmetic
                    selectedIndex = (selectedIndex + itemCount - 1) % itemCount;
                }
                if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S) {
                    selectedIndex = (selectedIndex + 1) % itemCount;
                }

                if (e.key.code == Keyboard::Enter || e.key.code == Keyboard::Space) {
                    // Return the action mapped to the currently selected item
                    final_action = static_cast<UI>(menuItems[selectedIndex].action);
                }
            }

            // MOUSE SELECTION
            if (e.type == Event::MouseMoved) {
                int mouseX = e.mouseMove.x;
                int mouseY = e.mouseMove.y;

                // Check for hover and update selection
                for (int i = 0; i < itemCount; ++i) {
                    if (isMouseOver(menuItems[i].bounds, mouseX, mouseY)) {
                        selectedIndex = i; // **HOVER HIGHLIGHT**
                        break;
                    }
                }
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                int mouseX = e.mouseButton.x;
                int mouseY = e.mouseButton.y;

                // Check for click and return action
                for (int i = 0; i < itemCount; ++i) {
                    if (isMouseOver(menuItems[i].bounds, mouseX, mouseY)) {
                        selectedIndex = i; // Ensure selection is updated before returning
                        final_action = static_cast<UI>(menuItems[selectedIndex].action); // **MOUSE CLICK SELECT**
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
            case SINGLE_PLAYER: // This is triggered by selecting a level
                return PLAY; // Exit the menu system and start the game!
            case UI::LEVEL_SELECT:
                setupLevelSelect();
                break;
            case UI::LEADERBOARD:
                setupLeaderBoard();
                break;
            case UI::QUIT:
                return EXIT; // Exit the entire game
                // Handling internal UI state transitions:
            case UI::START_UI: // START is triggered by "Play" in the main menu
                setupStartMenu();
                break;
            case UI::THEMES:
                switchTheme();  // Basically something I had done in a previous project that I wanted to add here
                break;
            case UI::BACK_MAIN: // MAIN is triggered by "Back" in sub-menus
                setupMainMenu();
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