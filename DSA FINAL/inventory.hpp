#pragma once
#include "theme.hpp"
#include "avl_tree.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

struct InventoryItem {
    Theme theme;
    bool unlocked;
    bool selected;

    InventoryItem() : unlocked(false), selected(false) {}
    InventoryItem(Theme t, bool u) : theme(t), unlocked(u), selected(false) {}
};

class Inventory {
private:
    AVLTree themeTree;
    InventoryItem* items;
    int capacity;
    int size;
    int selectedIndex;
    int scrollOffset;
    bool isActive;

    // Position variables
    float panelX, panelY;
    float panelWidth, panelHeight;

    // Visual elements
    RectangleShape background;
    RectangleShape itemBackground;
    Text titleText;
    Text itemText;
    Text descriptionText;
    Text statusText;
    Font font;

public:
    Inventory(float x = 20, float y = 110, float width = 320, float height = 320);
    ~Inventory();

    void initialize(AVLTree& tree);
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void loadThemes();
    void unlockTheme(int themeId);
    void selectTheme(int index);
    Theme getSelectedTheme();
    void scroll(int direction);
    void toggle();
    bool isInventoryActive() const { return isActive; }

    // Rendering
    void render(RenderWindow* window);
    void handleInput(Event& event);

private:
    void setupVisuals();
    void updatePositions(); // New method to update all positions
    int findThemeIndexById(int themeId);
};