// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#pragma once

class Grid {
public:
    Grid(int rows, int cols, int tileSize);
    ~Grid();

    int getRows() const;
    int getCols() const;
    int getCell(int y, int x) const;
    void setCell(int y, int x, int value);
    void drop(int y, int x);
    void clear();

private:
    int* grid;
    int M, N, ts;
};