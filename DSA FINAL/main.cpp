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


    MenuOptions choice = MENU;

    while (window.isOpen()) {

        if (choice == MENU) {
            choice = m.runMainMenu(true);
        }
        if (choice == PLAY) {
            Game g(&window);

            choice = g.run();
            if (choice == ENDMENU) {
                int gameScore = g.getScore();
                m.setScore(gameScore);
            }
        }
        if (choice == ENDMENU) {
            choice = m.runMainMenu(false);

            if (choice == PLAY) {
                continue;
            }
        }

        if (choice == EXIT) {
            break;
        }

    }

}
