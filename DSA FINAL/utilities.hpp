// Azlan Ali Khan 24I-2110, Sameer Hussain 24I-2042, DSA FINAL PROJECT
#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cctype>
using namespace sf;
using namespace std;

#include "grid.hpp"

extern const int M;
extern const int N;

extern const int ts; //tile size

enum MenuOptions {
    MENU,
    PLAY,
    LOAD_GAME,
    ENDMENU,
    EXIT
};

enum GameLevel {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3
};

enum GameMode {
    SINGLE_PLAYER_MODE,
    MULTIPLAYER_MODE
};
