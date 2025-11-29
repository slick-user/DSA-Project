// Azlan Ali Khan, Sameer Hussain 24I-2110, ? DSA FINAL PROJECT

#include <time.h>

#include "gameManager.hpp"
#include "utilities.hpp"
#include "enemy.hpp"
#include "game.hpp"
#include "menu.hpp"

RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game");

int main()
{
    srand(time(0));
    Menu m(&window);
 
    GameManager* gameManager;

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
        
        if (choice == PLAY || choice == LOAD_GAME) {
            // Pass current player information, selected level, and mode to Game
            Player currentPlayer = m.getCurrentPlayer();
            GameLevel selectedLevel = m.getSelectedLevel();
            GameMode selectedMode = m.getSelectedMode();
            
            cout << "DEBUG: Starting game for player: " << currentPlayer.username 
                 << " (ID: " << currentPlayer.playerID << ")" 
                 << " Level: " << (int)selectedLevel 
                 << " Mode: " << (selectedMode == MULTIPLAYER_MODE ? "Multiplayer" : "Single") << endl;
            
            string p2Name = "";
            int p2ID = 0;

            if (selectedMode == MULTIPLAYER_MODE) {
                p2Name = m.getPlayer2Username();
                p2ID = m.getPlayer2ID();
                cout << "DEBUG: Player 2: " << p2Name << " (ID: " << p2ID << ")" << endl;
            }
            
            Game* g = new Game(&window, currentPlayer.username, currentPlayer.playerID, 
                            selectedLevel, selectedMode, p2Name, p2ID, m.getBackgroundColor());
            
            if (choice == LOAD_GAME) {
                g->loadGame();
            }
            
            choice = g->run();
            
            if (choice == ENDMENU) {
                if (selectedMode == MULTIPLAYER_MODE) {
                    int p1Score = g->getPlayer1Score();
                    int p2Score = g->getPlayer2Score();
                    m.setMultiplayerScores(p1Score, p2Score);
                    cout << "DEBUG: Game ended - P1: " << p1Score << " P2: " << p2Score << endl;
                } else {
                    int gameScore = g->getScore();
                    m.setScore(gameScore);
                    cout << "DEBUG: Game ended with score: " << gameScore << endl;
                }
            }
            
            delete g;
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
    
    return 0;
}