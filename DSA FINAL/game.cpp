// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#include "game.hpp"

Game::Game(RenderWindow* window) : window(window),
               x(0), y(0), dx(0), dy(0), timer(0), delay(0.07),
               GameRunning(true), enemyCount(4), score(-1748),
               bonusCounter(0), tilesCapturedThisMove(0),
               powerUpCount(0), freezePowerUpActive(false),
               freezePowerUpTimer(0), displayTimer(0), lastPointsEarned(0) {

    window->setFramerateLimit(60);

    tTile.loadFromFile("assets/tiles.png");
    tGameover.loadFromFile("assets/gameover.png");
    tEnemy.loadFromFile("assets/enemy.png");

    sTile.setTexture(tTile);
    sGameover.setTexture(tGameover);
    sEnemy.setTexture(tEnemy);
    sEnemy.setOrigin(20, 20);


    if (!font.loadFromFile("assets/monogram.ttf")) {
        cout << "Font failed to load!";
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);

    powerUpText.setFont(font);
    powerUpText.setCharacterSize(20);
    powerUpText.setFillColor(Color::Magenta);
    powerUpText.setPosition(10, 40);

    bonusText.setFont(font);
    bonusText.setCharacterSize(18);
    bonusText.setFillColor(Color::Red);
    bonusText.setPosition(10, 65);

    freezeTimerText.setFont(font);
    freezeTimerText.setCharacterSize(20);
    freezeTimerText.setFillColor(Color::Blue);
    freezeTimerText.setPosition(10, 90);

    // initialize Grid
    grid = new Grid(M, N, ts);

    // Initialize Enemies
    for (int i = 0; i < enemyCount; i++)
        enemies[i] = Enemy(rand() % (N * ts), rand() % (M * ts), grid);
}

int Game::getScore() const { return score; }
int Game::getPowerUpCount() const { return powerUpCount; }

int Game::addScore(int tilesCaptured) {
    tilesCapturedThisMove = tilesCaptured;
    int points = 0;

    if (tilesCaptured <= 0) {
        lastPointsEarned = 0;
        return 0;
    }

    // Base points: 1 point per tile
    points = tilesCaptured;
    int multiplier = 1;

    // Bonus system (decide multiplier first)
    if (bonusCounter < 3) {
        // First threshold: >10 tiles for ×2 bonus
        if (tilesCaptured > 10) {
            multiplier = 2;
            bonusCounter++;
        }
    }
    else if (bonusCounter < 5) {
        // Second threshold: >5 tiles for ×2 bonus (after 3 occurrences)
        if (tilesCaptured > 5) {
            multiplier = 2;
            bonusCounter++;
        }
    }
    else {
        // Final threshold: >5 tiles for ×4 bonus (after 5 total occurrences)
        if (tilesCaptured > 5) {
            multiplier = 4;
            bonusCounter++;
        }
    }

    points *= multiplier;
    score += points;
    checkPowerUpAward();

    lastPointsEarned = points;
    return points;
}

void Game::checkPowerUpAward() {
    // Check if player reached score milestones for power-ups
    if (score >= 50 && powerUpCount == 0) {
        powerUpCount++;
    }
    else if (score >= 70 && powerUpCount == 1) {
        powerUpCount++;
    }
    else if (score >= 100 && powerUpCount == 2) {
        powerUpCount++;
    }
    else if (score >= 130 && powerUpCount == 3) {
        powerUpCount++;
    }
    else if (score >= (130 + (powerUpCount - 3) * 30) && powerUpCount >= 4) {
        // Every additional 30 points after 130
        powerUpCount++;
    }
}

void Game::activateFreezePowerUp() {
    if (powerUpCount > 0 && !freezePowerUpActive) {
        powerUpCount--;
        freezePowerUpActive = true;
        freezePowerUpTimer = 3.0f; // 3 seconds
    }
}

void Game::updateFreezePowerUp(float dt) {
    if (freezePowerUpActive) {
        freezePowerUpTimer -= dt;
        if (freezePowerUpTimer <= 0) {
            freezePowerUpActive = false;
        }
    }
}

int Game::countCapturedTiles() {
    int capturedTiles = 0;
    // Count total playable cells (excluding borders)
    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getCols(); j++) {
            if (grid->getCell(i, j) == -1) {  // Filled cells
                capturedTiles++;
            }
        }
    }
    return capturedTiles;
}

MenuOptions Game::run() {
    Clock clock;

    while (window->isOpen() && GameRunning) {
        float dt = clock.restart().asSeconds();
        timer += dt;

        processInput();
        if (GameRunning) {
            update(dt);
            updateFreezePowerUp(dt);
        }
        render();

    }

    sleep(seconds(1));

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

        // Activating freeze powerup
        else if (e.key.code == Keyboard::Space) {
            activateFreezePowerUp();
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

    // Update display timer for UI
    if (tilesCapturedThisMove > 0) {
        displayTimer += dt;
        if (displayTimer > 3.0f) {
            tilesCapturedThisMove = 0;
            displayTimer = 0;
        }
    }

    if (!freezePowerUpActive) {
        for (int i = 0; i < enemyCount; i++)
            enemies[i].move();
    }

    static bool wasCapturing = false;
    int currentCell = grid->getCell(y, x);

    if (currentCell == 1 && !wasCapturing) {
        cout << "CAPTURE TRIGGERED!" << endl;

        wasCapturing = true;

        // run drop for each enemy so inside/outside is determined
        for (int i = 0; i < enemyCount; i++)
            grid->drop(enemies[i].getY() / ts, enemies[i].getX() / ts);

        // convert -1 -> 0 (captured) and everything else -> 1 (playfield)
        int newlyCaptured = 0;
        for (int i = 0; i < grid->getRows(); i++) {
            for (int j = 0; j < grid->getCols(); j++) {
                if (grid->getCell(i, j) == -1) {
                    grid->setCell(i, j, 0);
                    newlyCaptured++;
                }
                else {
                    grid->setCell(i, j, 1);
                }
            }
        }

        cout << "Tiles captured this operation: " << newlyCaptured << endl;

        if (newlyCaptured > 0) {
            // Now score using the correct count (and record awarded points)
            int awarded = addScore(newlyCaptured);
            cout << "Points awarded: " << awarded << "; Score updated to: " << score << endl;

            displayTimer = 0;
        }
    }
    else if (currentCell != 1) {
        wasCapturing = false;
    }

    // Check enemy collision (only if not frozen)
    if (!freezePowerUpActive) {
        for (int i = 0; i < enemyCount; i++)
            if (grid->getCell(enemies[i].getY() / ts, enemies[i].getX() / ts) == 2)
                GameRunning = false;
    }
}

void Game::drawUI() {
    // Update score text
    scoreText.setString("Score: " + to_string(score));
    window->draw(scoreText);

    // Update power-up text
    powerUpText.setString("Power-ups: " + to_string(powerUpCount) + " (Press SPACE to use)");
    window->draw(powerUpText);

    // Update bonus system status
    string bonusStatus = "Bonus: ";
    if (bonusCounter < 3) {
        bonusStatus += ">10 tiles = x2 (" + to_string(bonusCounter) + "/3)";
    }
    else if (bonusCounter < 5) {
        bonusStatus += ">5 tiles = x2 (" + to_string(bonusCounter - 3) + "/2)";
    }
    else {
        bonusStatus += ">5 tiles = x4 (MAX)";
    }
    bonusText.setString(bonusStatus);
    window->draw(bonusText);

    // Show freeze timer if active
    if (freezePowerUpActive) {
        freezeTimerText.setString("FREEZE: " + to_string(static_cast<int>(freezePowerUpTimer + 0.99f)) + "s");
        window->draw(freezeTimerText);
    }

    // Show tiles captured in last move (temporary display)
    if (tilesCapturedThisMove > 0) {
        Text moveText;
        moveText.setFont(font);
        moveText.setCharacterSize(16);
        moveText.setFillColor(Color::Green);
        moveText.setPosition(10, 120);

        std::string moveString = "Last capture: " + to_string(tilesCapturedThisMove) + " tiles";

        // Calculate points earned
        int pointsEarned = lastPointsEarned;
        if (pointsEarned > 0) {
            int multiplier = pointsEarned / tilesCapturedThisMove;
            if (multiplier > 1) {
                moveString += " (x" + std::to_string(multiplier) + " BONUS!)";
            }
        }

        moveString += " = " + std::to_string(pointsEarned) + " points";
        moveText.setString(moveString);
        window->draw(moveText);
    }
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
    if (!freezePowerUpActive) {
        sEnemy.rotate(10);
        for (int i = 0; i < enemyCount; i++) {
            sEnemy.setPosition(enemies[i].getX(), enemies[i].getY());
            window->draw(sEnemy);
        }
    } else {
        sEnemy.setColor(Color::Blue); // Blue tint for frozen enemies
        for (int i = 0; i < enemyCount; i++) {
            sEnemy.setPosition(enemies[i].getX(), enemies[i].getY());
            window->draw(sEnemy);
        }
        sEnemy.setColor(Color::White);
    }

    drawUI();

    if (!GameRunning) {
        GameManager::getInstance()->updatePlayerStats(score);
        window->draw(sGameover);
    }

    window->display();
}
