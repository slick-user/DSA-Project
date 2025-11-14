// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#include <time.h>

#include "utilities.hpp"
#include "enemy.hpp"
#include "game.hpp"
#include "menu.hpp"

RenderWindow window(VideoMode(N* ts, M* ts), "Xonix Game");

int main()
{
    srand(time(0));
    Menu m(&window);

    while (window.isOpen()) {
        
        MenuOptions choice = m.runMainMenu();

        if (choice == MENU) {
            m.runMainMenu();
        }
        if (choice == PLAY) {
            Game g(&window);
            g.run();
        }

        if (choice == EXIT) {
            break;
        }

    }

}
