// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#include "game.hpp"

Game::Game(RenderWindow* window) : window(window),
               x(0), y(0), dx(0), dy(0), timer(0), delay(0.07),
               GameRunning(true), enemyCount(4), score(0) {

    window->setFramerateLimit(60);

    tTile.loadFromFile("assets/tiles.png");
    tGameover.loadFromFile("assets/gameover.png");
    tEnemy.loadFromFile("assets/enemy.png");

    sTile.setTexture(tTile);
    sGameover.setTexture(tGameover);
    sEnemy.setTexture(tEnemy);
    sEnemy.setOrigin(20, 20);

    // initialize Grid
    grid = new Grid(M, N, ts);

    // Initialize Enemies
    for (int i = 0; i < enemyCount; i++)
        enemies[i] = Enemy(rand() % (N * ts), rand() % (M * ts), grid);
}

int Game::getScore() const { return score; }

void Game::calculateScore() {
    int totalCells = 0;
    int filledCells = 0;

    // Count total playable cells (excluding borders)
    for (int i = 1; i < grid->getRows() - 1; i++) {
        for (int j = 1; j < grid->getCols() - 1; j++) {
            totalCells++;
            if (grid->getCell(i, j) == 1) {  // Filled cells
                filledCells++;
            }
        }
    }

    // Calculate percentage and convert to score (0-10000 points)
    if (totalCells > 0) {
        float percentage = (float)filledCells / totalCells * 100.0f;
        score = (int)(percentage * 100);  // Score from 0 to 10000
    }
}

MenuOptions Game::run() {
    Clock clock;

    while (window->isOpen() && GameRunning) {
        float dt = clock.restart().asSeconds();
        timer += dt;

        processInput();
        if (GameRunning) update(dt);
        render();

    }

    sleep(seconds(1));

    calculateScore();

    return ENDMENU;
}

void Game::processInput() {
    Event e;
    while (window->pollEvent(e)) {
        if (e.type == Event::Closed)
            window->close();

        if (e.type == Event::KeyPressed &&
            e.key.code == Keyboard::Escape) {
            grid->clear(); // reset grid cells
            x = 10; y = 0;
            GameRunning = true;
        }
    }

    // So these are player inputs probably need to add a player class
    if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; }
    if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1;  dy = 0; }
    if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0;  dy = -1; }
    if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0;  dy = 1; }
}

void Game::update(float dt) {
    if (timer >= delay) {
        x += dx;
        y += dy;

        if (x < 0) x = 0; if (x > grid->getCols() - 1) x = grid->getCols() - 1;
        if (y < 0) y = 0; if (y > grid->getRows() - 1) y = grid->getRows() - 1;

        if (grid->getCell(y, x) == 2) GameRunning = false;
        if (grid->getCell(y, x) == 0) grid->setCell(y, x, 2);
        timer = 0;
    }

    for (int i = 0; i < enemyCount; i++)
        enemies[i].move();

    if (grid->getCell(y, x) == 1) {
        dx = dy = 0;

        for (int i = 0; i < enemyCount; i++)
            grid->drop(enemies[i].getY() / ts, enemies[i].getX() / ts);

        for (int i = 0; i < grid->getRows(); i++)
            for (int j = 0; j < grid->getCols(); j++)
                grid->setCell(i, j, (grid->getCell(i, j) == -1) ? 0 : 1);
    }

    for (int i = 0; i < enemyCount; i++)
        if (grid->getCell(enemies[i].getY() / ts, enemies[i].getX() / ts) == 2)
            GameRunning = false;
}

void Game::render() {
    window->clear();

    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getCols(); j++) {
            int cell = grid->getCell(i, j);
            if (cell == 0) continue;

            if (cell == 1) sTile.setTextureRect({ 0, 0, ts, ts });
            else if (cell == 2) sTile.setTextureRect({ 54, 0, ts, ts });

            sTile.setPosition(j * ts, i * ts);
            window->draw(sTile);
        }
    }

    // Draw Player
    sTile.setTextureRect({ 36,0,ts,ts });
    sTile.setPosition(x * ts, y * ts);
    window->draw(sTile);

    // Draw Enemy
    sEnemy.rotate(10);
    for (int i = 0; i < enemyCount; i++) {
        sEnemy.setPosition(enemies[i].getX(), enemies[i].getY());
        window->draw(sEnemy);
    }

    if (!GameRunning) {
        window->draw(sGameover);
    }

    window->display();
}
