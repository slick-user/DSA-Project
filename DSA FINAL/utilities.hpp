// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

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
#include <sstream>
using namespace sf;
using namespace std;

#include "grid.hpp"

extern const int M;
extern const int N;

extern const int ts; //tile size

enum MenuOptions {
    MENU,
    PLAY,
    ENDMENU,
    EXIT
};
