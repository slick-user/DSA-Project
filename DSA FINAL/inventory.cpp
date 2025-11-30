#include "inventory.hpp"
#include <iostream>

Inventory::Inventory(float x, float y, float width, float height)
    : capacity(20), size(0), selectedIndex(0), scrollOffset(0), isActive(false),
    panelX(x), panelY(y), panelWidth(width), panelHeight(height),
    isSearching(false), searchQuery("") {
    items = new InventoryItem[capacity];
    setupVisuals();
}

Inventory::~Inventory() {
    delete[] items;
}

void Inventory::setPosition(float x, float y) {
    panelX = x;
    panelY = y;
    updatePositions();
}

void Inventory::setSize(float width, float height) {
    panelWidth = width;
    panelHeight = height;
    updatePositions();
}

void Inventory::updatePositions() {
    // Update background
    background.setSize(Vector2f(panelWidth, panelHeight));
    background.setPosition(panelX, panelY);

    // Update item background size based on panel width
    itemBackground.setSize(Vector2f(panelWidth - 20, 30));

    // Update title position (centered above panel)
    titleText.setPosition(panelX + (panelWidth - titleText.getLocalBounds().width) / 2, panelY - 25);

}

void Inventory::setupVisuals() {
    if (!font.loadFromFile("assets/monogram.ttf")) {
        std::cerr << "Failed to load font for inventory!" << std::endl;
    }

    // Background
    background.setSize(Vector2f(panelWidth, panelHeight));
    background.setPosition(panelX, panelY);
    background.setFillColor(Color(30, 30, 40, 230));
    background.setOutlineThickness(2);
    background.setOutlineColor(Color(100, 100, 150));

    // Item background
    itemBackground.setSize(Vector2f(panelWidth - 20, 30));
    itemBackground.setFillColor(Color(50, 50, 70, 180));
    itemBackground.setOutlineThickness(1);
    itemBackground.setOutlineColor(Color(80, 80, 110));

    // Text setup
    titleText.setFont(font);
    titleText.setCharacterSize(20);
    titleText.setString("THEME INVENTORY");
    titleText.setPosition(panelX + (panelWidth - titleText.getLocalBounds().width) / 2, panelY - 25);
    titleText.setFillColor(Color::Yellow);

    itemText.setFont(font);
    itemText.setCharacterSize(14);

    descriptionText.setFont(font);
    descriptionText.setCharacterSize(12);

    statusText.setFont(font);
    statusText.setCharacterSize(12);

    // Search box setup
    searchBox.setSize(Vector2f(panelWidth - 100, 25));
    searchBox.setFillColor(Color(40, 40, 50));
    searchBox.setOutlineThickness(1);
    searchBox.setOutlineColor(Color(100, 100, 150));

    // Search text setup
    searchText.setFont(font);
    searchText.setCharacterSize(14);
    searchText.setFillColor(Color::White);
}

void Inventory::initialize(AVLTree& tree) {
    themeTree = tree;
    loadThemes();
}

void Inventory::loadThemes() {
    Theme* allThemes = new Theme[themeTree.getSize()];
    int themeCount = 0;
    themeTree.getAllThemes(allThemes, themeCount);

    size = min(themeCount, capacity);

    for (int i = 0; i < size; i++) {
        items[i] = InventoryItem(allThemes[i], false);
    }

    delete[] allThemes;

    for (int i = 0; i < min(capacity, size); i++) {
        items[i].unlocked = true;
    }

    // Auto-select first unlocked theme
    for (int i = 0; i < size; i++) {
        if (items[i].unlocked) {
            selectTheme(i);
            break;
        }
    }
}

void Inventory::unlockTheme(int themeId) {
    int index = findThemeIndexById(themeId);
    if (index != -1) {
        items[index].unlocked = true;
    }
}

void Inventory::selectTheme(int index) {
    if (index >= 0 && index < size && items[index].unlocked) {
        // Deselect all
        for (int i = 0; i < size; i++) {
            items[i].selected = false;
        }
        // Select new one
        items[index].selected = true;
        selectedIndex = index;
    }
}

Theme Inventory::getSelectedTheme() {
    if (selectedIndex >= 0 && selectedIndex < size && items[selectedIndex].unlocked) {
        return items[selectedIndex].theme;
    }
    return Theme(); // Return default theme
}

void Inventory::scroll(int direction) {
    if (direction > 0 && selectedIndex < size - 1) {
        selectedIndex++;
        // Adjust scroll offset to keep selected item visible (show 4 items at a time)
        if (selectedIndex >= scrollOffset + 4) {
            scrollOffset++;
        }
    }
    else if (direction < 0 && selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < scrollOffset) {
            scrollOffset--;
        }
    }
}

void Inventory::toggle() {
    isActive = !isActive;
    // Reset search state when closing inventory
    if (!isActive) {
        isSearching = false;
        searchQuery = "";
    }
}

// Search functionality implementation
void Inventory::startSearch() {
    isSearching = true;
    searchQuery = "";
    searchText.setString("> " + searchQuery);
}

void Inventory::handleSearchInput(Event& event) {
    if (!isSearching) return;

    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Enter) {
            // Perform search when Enter is pressed
            performSearch();
            isSearching = false;
        }
        else if (event.key.code == Keyboard::Escape) {
            // Cancel search
            isSearching = false;
            searchQuery = "";
        }
        else if (event.key.code == Keyboard::BackSpace) {
            // Handle backspace
            if (!searchQuery.empty()) {
                searchQuery.pop_back();
                searchText.setString("> " + searchQuery);
            }
        }
        // Handle number keys in KeyPressed event
        else if (event.key.code >= Keyboard::Num0 && event.key.code <= Keyboard::Num9) {
            // Convert key code to actual number character
            char numberChar = '0' + (event.key.code - Keyboard::Num0);
            searchQuery += numberChar;
            searchText.setString("> " + searchQuery);
        }
    }
    else if (event.type == Event::TextEntered) {
        // Handle text input (numbers only for ID search)
        if (event.text.unicode >= '0' && event.text.unicode <= '9') {
            searchQuery += static_cast<char>(event.text.unicode);
            searchText.setString("> " + searchQuery);
        }
    }
}

void Inventory::performSearch() {
    if (searchQuery.empty()) return;

    int searchId = std::stoi(searchQuery);

    // Use the AVL tree's search functionality
    Theme* foundTheme = themeTree.search(searchId);

    if (foundTheme != nullptr) {
        // Find the index of the theme in our items array
        int foundIndex = -1;
        for (int i = 0; i < size; i++) {
            if (items[i].theme.id == searchId) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            // Found the theme, select it and ensure it's visible
            selectTheme(foundIndex);

            // Adjust scroll to make the found item visible
            if (foundIndex < scrollOffset) {
                scrollOffset = foundIndex;
            }
            else if (foundIndex >= scrollOffset + 4) {
                scrollOffset = foundIndex - 3;
            }
        }
    }
}

void Inventory::render(RenderWindow* window) {
    if (!isActive) return;

    window->draw(background);

    if (isSearching) {
        // Center the search interface vertically and horizontally
        float searchBoxWidth = panelWidth - 40;
        float searchBoxHeight = 30;
        float searchBoxX = panelX + 20;
        float searchBoxY = panelY + (panelHeight - searchBoxHeight) / 2 - 20;

        // Update search box size and position for centered look
        searchBox.setSize(Vector2f(searchBoxWidth, searchBoxHeight));
        searchBox.setPosition(searchBoxX, searchBoxY);
        window->draw(searchBox);

        // Position search text inside the box
        searchText.setPosition(searchBoxX + 10, searchBoxY + 5);
        window->draw(searchText);

        // Search title above the box
        Text searchTitle("SEARCH THEME BY ID", font, 16);
        searchTitle.setFillColor(Color::Yellow);
        searchTitle.setPosition(panelX + (panelWidth - searchTitle.getLocalBounds().width) / 2, panelY + 30);
        window->draw(searchTitle);

        // Search instructions below the box
        Text instructions("Enter theme ID number and press ENTER", font, 12);
        instructions.setFillColor(Color::Cyan);
        instructions.setPosition(panelX + (panelWidth - instructions.getLocalBounds().width) / 2, searchBoxY + 40);
        window->draw(instructions);

        Text instructions2("Press ESC to cancel", font, 12);
        instructions2.setFillColor(Color(150, 150, 150));
        instructions2.setPosition(panelX + (panelWidth - instructions2.getLocalBounds().width) / 2, searchBoxY + 60);
        window->draw(instructions2);

        return;
    }

    // Normal inventory rendering (rest of your existing code remains the same)
    window->draw(titleText);

    // Calculate positions relative to panel
    float itemStartY = panelY + 10; // Changed back to original position
    float itemSpacing = 35;
    int visibleItems = min(4, size - scrollOffset);

    for (int i = 0; i < visibleItems; i++) {
        int itemIndex = i + scrollOffset;
        if (itemIndex >= size) break;

        float yPos = itemStartY + i * itemSpacing;

        // Item background
        itemBackground.setPosition(panelX + 10, yPos);

        // Color based on status
        if (itemIndex == selectedIndex) {
            itemBackground.setFillColor(Color(80, 80, 120, 200));
        }
        else if (!items[itemIndex].unlocked) {
            itemBackground.setFillColor(Color(60, 40, 40, 180));
        }
        else {
            itemBackground.setFillColor(Color(50, 50, 70, 180));
        }

        window->draw(itemBackground);

        // Theme name
        itemText.setPosition(panelX + 20, yPos + 6);
        string displayName = items[itemIndex].theme.name;
        if (displayName.length() > 15) {
            displayName = displayName.substr(0, 15) + "...";
        }

        if (!items[itemIndex].unlocked) {
            itemText.setFillColor(Color(150, 150, 150));
            itemText.setString(displayName + " [L]");
        }
        else if (items[itemIndex].selected) {
            itemText.setFillColor(Color::Yellow);
            itemText.setString(displayName + " ✓");
        }
        else {
            itemText.setFillColor(Color::White);
            itemText.setString(displayName);
        }

        window->draw(itemText);

        // Theme ID on the right side
        itemText.setPosition(panelX + panelWidth - 40, yPos + 6);
        itemText.setString("#" + to_string(items[itemIndex].theme.id));
        itemText.setFillColor(Color(200, 200, 200));
        window->draw(itemText);
    }

    // Display current selection details
    if (selectedIndex >= 0 && selectedIndex < size) {
        Theme selected = items[selectedIndex].theme;

        // Theme preview section
        float previewY = panelY + panelHeight - 90;
        Text previewTitle("Preview:", font, 14);
        previewTitle.setPosition(panelX + 10, previewY);
        previewTitle.setFillColor(Color::Cyan);
        window->draw(previewTitle);

        // Theme preview colors
        RectangleShape colorPreview;
        colorPreview.setSize(Vector2f(20, 20));

        // Color swatches
        colorPreview.setPosition(panelX + 10, previewY + 20);
        colorPreview.setFillColor(selected.bgColor);
        window->draw(colorPreview);

        colorPreview.setPosition(panelX + 40, previewY + 20);
        colorPreview.setFillColor(selected.titleColor);
        window->draw(colorPreview);

        colorPreview.setPosition(panelX + 70, previewY + 20);
        colorPreview.setFillColor(selected.textColor);
        window->draw(colorPreview);

        colorPreview.setPosition(panelX + 100, previewY + 20);
        colorPreview.setFillColor(selected.highlightColor);
        window->draw(colorPreview);

        colorPreview.setPosition(panelX + 130, previewY + 20);
        colorPreview.setFillColor(selected.buttonColor);
        window->draw(colorPreview);

        // Status text
        statusText.setPosition(panelX + 10, previewY + 50);
        if (items[selectedIndex].selected) {
            statusText.setString("Active - ENTER to confirm");
            statusText.setFillColor(Color::Green);
        }
        else {
            statusText.setString("ENTER to select");
            statusText.setFillColor(Color::Yellow);
        }
        window->draw(statusText);
    }

    // Instructions below the panel (updated to include search)
    float instructionsY = panelY + panelHeight + 10;
    Text instructions("UP/DOWN: Navigate  ENTER: Select  F: Search", font, 10);
    instructions.setPosition(panelX, instructionsY);
    instructions.setFillColor(Color(150, 150, 150));
    window->draw(instructions);

    Text instructions2("ESC: Close", font, 10);
    instructions2.setPosition(panelX, instructionsY + 15);
    instructions2.setFillColor(Color(150, 150, 150));
    window->draw(instructions2);

    // Scroll indicator
    if (size > 4) {
        Text scrollText;
        scrollText.setFont(font);
        scrollText.setString("Items " + to_string(scrollOffset + 1) + "-" +
            to_string(min(scrollOffset + 4, size)) + " of " + to_string(size));
        scrollText.setCharacterSize(10);
        scrollText.setPosition(panelX, instructionsY + 35);
        scrollText.setFillColor(Color(150, 150, 150));
        window->draw(scrollText);
    }
}

void Inventory::handleInput(Event& event) {
    if (!isActive) return;

    // Handle search input first if searching
    if (isSearching) {
        handleSearchInput(event);
    }

    if (event.type == Event::KeyPressed) {
        switch (event.key.code) {
        case Keyboard::Up:
        case Keyboard::W:
            if (!isSearching) scroll(-1); // Only scroll if not searching
            break;
        case Keyboard::Down:
        case Keyboard::S:
            if (!isSearching) scroll(1); // Only scroll if not searching
            break;
        case Keyboard::Enter:
        case Keyboard::Space:
            if (!isSearching) selectTheme(selectedIndex);
            // If searching, Enter is handled in handleSearchInput
            break;
        case Keyboard::F:  // Add search key (F for find)
            if (!isSearching) startSearch();
            break;
        case Keyboard::Escape:
            if (isSearching) {
                // Cancel search
                isSearching = false;
                searchQuery = "";
            }
            else {
                toggle();
            }
            break;
        default:
            break;
        }
    }

    if (event.type == Event::MouseWheelScrolled && !isSearching) {
        if (event.mouseWheelScroll.delta > 0) {
            scroll(-1);
        }
        else if (event.mouseWheelScroll.delta < 0) {
            scroll(1);
        }
    }
}

int Inventory::findThemeIndexById(int themeId) {
    for (int i = 0; i < size; i++) {
        if (items[i].theme.id == themeId) {
            return i;
        }
    }
    return -1;
}