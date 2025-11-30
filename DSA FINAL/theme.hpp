// Azlan Ali Khan 24I-2110, Sameer Hussain 24I-2042, DSA FINAL PROJECT
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

struct Theme {
    string name;
    Color bgColor;
    Color titleColor;
    Color textColor;
    Color highlightColor;
    Color buttonColor;
    int id; // For AVL tree comparison

    // Default constructor
    Theme() : id(-1), name("Default"), bgColor(Color::Black), titleColor(Color::White),
        textColor(Color::White), highlightColor(Color::Yellow), buttonColor(Color::Blue) {}

    // Parameterized constructor
    Theme(int id, const string& name, Color bg, Color title, Color text, Color highlight, Color button)
        : id(id), name(name), bgColor(bg), titleColor(title), textColor(text),
        highlightColor(highlight), buttonColor(button) {}

    // Overload operators for AVL tree comparison
    bool operator<(const Theme& other) const {
        return id < other.id;
    }
    bool operator>(const Theme& other) const {
        return id > other.id;
    }
    bool operator==(const Theme& other) const {
        return id == other.id;
    }
};
