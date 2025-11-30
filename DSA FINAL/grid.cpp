// Azlan Ali Khan 24I-2110, Sameer Hussain 24I-2042, DSA FINAL PROJECT
#include "grid.hpp"

Grid::Grid(int rows, int cols, int tileSize) : M(rows), N(cols), ts(tileSize) {
    grid = new int[M * N]();

    // Initialize borders to 1 (walls)
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            grid[i * N + j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1) ? 1 : 0;
}

Grid::~Grid() {
    delete[] grid;
}

int Grid::getRows() const { return M; }

int Grid::getCols() const { return N; }

int Grid::getCell(int y, int x) const {
    if (y < 0 || y >= M || x < 0 || x >= N) return -1; // bounds check
    return grid[y * N + x];
}

void Grid::setCell(int y, int x, int value) {
    if (y < 0 || y >= M || x < 0 || x >= N) return;
    grid[y * N + x] = value;
}

void Grid::drop(int y, int x) {
    if (getCell(y, x) != 0) return;

    setCell(y, x, -1);

    drop(y - 1, x);
    drop(y + 1, x);
    drop(y, x - 1);
    drop(y, x + 1);
}

// Clears only inner area, keeps border walls intact
void Grid::clear() {
    for (int i = 1; i < M - 1; i++)
        for (int j = 1; j < N - 1; j++)
            grid[i * N + j] = 0;
}
