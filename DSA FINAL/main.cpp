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
    
    // Show auth screen on boot if not logged in
    if (!m.isLoggedIn()) {
        choice = m.runAuthScreen();
        // After auth screen, if still not logged in, exit
        if (!m.isLoggedIn() && choice != MENU) {
            return 0;
        }
    }
    
    while (window.isOpen()) {
        if (choice == MENU) {
            // Only show main menu if logged in
            if (m.isLoggedIn()) {
                choice = m.runMainMenu(true);
            } else {
                // Not logged in, show auth screen again
                choice = m.runAuthScreen();
                if (!m.isLoggedIn() && choice != MENU) {
                    break;
                }
            }
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
