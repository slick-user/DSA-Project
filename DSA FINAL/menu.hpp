#include "utilities.hpp"

const int MAX_MENU_ITEMS = 6;

enum UI {
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
	QUIT
};

/*
enum START_OPTIONS {
	SINGLEPLAYER,
	MULTIPLAYER,
	BACK
};
*/

struct MenuItem {
	Text text;
	UI action; // the enum value to return to when this item is selected
	FloatRect bounds;
};

class Menu {
public:
	Menu(RenderWindow* window = nullptr);

	MenuOptions runMainMenu();

private:
	RenderWindow* window;
	Font font;
	Text title;
	
	int fontSize = 45;

	MenuItem menuItems[MAX_MENU_ITEMS];
	int itemCount;
	int selectedIndex;

	UI currentMenuType;

	int theme;
	Color bgColor;
	Color fontColor = { 255, 255, 255 };

	void setupMainMenu();
	// UI's 
	void setupStartMenu();
	void setupLevelSelect();
	void setupLeaderBoard();

	void switchTheme();

	void handleInput(const Event& e);
	void updateSelection(const Event& e);
	void draw();

	// Utility for mouse-over check
	bool isMouseOver(const FloatRect& bounds, int mouseX, int mouseY) const;

};