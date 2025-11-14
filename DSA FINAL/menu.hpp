#include "utilities.hpp"

class Menu {
public:
	Menu(RenderWindow* window = nullptr);

	MenuOptions runMainMenu();

private:
	RenderWindow* window;
};