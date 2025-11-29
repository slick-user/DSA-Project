// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

#include "game.hpp"

Game::Game(RenderWindow* window, const string& username, int playerID,
           GameLevel level, GameMode mode, const string& username2, int playerID2, Color bgColor)
           : window(window), x(0), y(0), dx(0), dy(0), timer(0),
             GameRunning(true), score(0), currentUsername(username),
             currentPlayerID(playerID), player2Username(username2), player2PlayerID(playerID2),
             currentLevel(level), currentMode(mode), isMultiplayer(mode == MULTIPLAYER_MODE),
             enemiesFrozen(false), enemyFreezeTimer(0.0f), player1(nullptr), player2(nullptr),
             bgColor(bgColor), lastPointsEarned(0), bonusCounter(0), tilesCapturedThisMove(0) {

    window->setFramerateLimit(60);

    tTile.loadFromFile("assets/tiles.png");
    tGameover.loadFromFile("assets/gameover.png");
    tEnemy.loadFromFile("assets/enemy.png");

    sTile.setTexture(tTile);
    sGameover.setTexture(tGameover);
    sEnemy.setTexture(tEnemy);
    sEnemy.setOrigin(20, 20);

    // Initialize Grid
    grid = new Grid(M, N, ts);

    // Set level parameters
    switch (level) {
    case EASY:
        enemyCount = 3;
        delay = 0.1f;
        targetPercentage = 0.75f; // 75% for easy
        break;
    case MEDIUM:
        enemyCount = 5;
        delay = 0.08f;
        targetPercentage = 0.80f; // 80% for medium
        break;
    case HARD:
        enemyCount = 7;
        delay = 0.05f;
        targetPercentage = 0.85f; // 85% for hard
        break;
    default:
        enemyCount = 3;
        delay = 0.1f;
        targetPercentage = 0.75f;
    }

    // Initialize Enemies dynamically
    enemies = new Enemy[enemyCount];
    for (int i = 0; i < enemyCount; i++)
        enemies[i] = Enemy(rand() % (N * ts), rand() % (M * ts), grid);

    // Player 1 Initialization
    player1 = new GamePlayer(x, y, 1, grid);

    if (isMultiplayer) {
        // Player 2: Bottom-right corner  
        player2 = new GamePlayer(N, M, 2, grid);
    }

    gameManager = GameManager::getInstance();

    // Initialize leaderboard
    leaderboard = new Leaderboard();
}

Game::~Game() {
    delete grid;
    delete[] enemies;
    delete leaderboard;
    if (player1) delete player1;
    if (player2) delete player2;
}

int Game::getScore() const { return score; }
int Game::getPlayer1Score() const { return player1 ? player1->getScore() : 0; }
int Game::getPlayer2Score() const { return player2 ? player2->getScore() : 0; }

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

    // Bonus system
    if (bonusCounter < 3) {
        if (tilesCaptured > 10) {
            multiplier = 2;
            bonusCounter++;
        }
    }
    else if (bonusCounter < 5) {
        if (tilesCaptured > 5) {
            multiplier = 2;
            bonusCounter++;
        }
    }
    else {
        if (tilesCaptured > 5) {
            multiplier = 4;
            bonusCounter++;
        }
    }

    points *= multiplier;
    score += points;

    // Update player1's score in single player mode
    if (!isMultiplayer && player1) {
        player1->setScore(score);
    }

    // Check power-up award - always use player1 for single player
    checkPowerUpAward(isMultiplayer ? nullptr : player1);

    lastPointsEarned = points;
    return points;
}

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
        
        // Bonus for reaching target
        if (percentage >= targetPercentage) {
            score += 1000 * (int)currentLevel;
        }
    }
}

void Game::checkPowerUpAward(GamePlayer* p) {
    if (!p) {
        p = player1;
    }

    if (p) {
        int playerScore = p->getScore();
        int currentPowerUps = p->getPowerUpCount();

        if (playerScore >= 50 && currentPowerUps == 0) {
            p->incrementPowerUpCount(1);
            cout << "Power-up awarded! Total: " << p->getPowerUpCount() << endl;
        }
        else if (playerScore >= 70 && currentPowerUps == 1) {
            p->incrementPowerUpCount(1);
            cout << "Power-up awarded! Total: " << p->getPowerUpCount() << endl;
        }
        else if (playerScore >= 100 && currentPowerUps == 2) {
            p->incrementPowerUpCount(1);
            cout << "Power-up awarded! Total: " << p->getPowerUpCount() << endl;
        }
        else if (playerScore >= 130 && currentPowerUps == 3) {
            p->incrementPowerUpCount(1);
            cout << "Power-up awarded! Total: " << p->getPowerUpCount() << endl;
        }
        else if (playerScore >= (130 + (currentPowerUps - 3) * 30) && currentPowerUps >= 4) {
            p->incrementPowerUpCount(1);
            cout << "Power-up awarded! Total: " << p->getPowerUpCount() << endl;
        }
    }
}

int Game::countCapturedTiles() {
    int capturedTiles = 0;

    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getCols(); j++) {
            if (grid->getCell(i, j) == 1) { 
                capturedTiles++;
            }
        }
    }
    return capturedTiles;
}

int Game::countNewlyCapturedTiles() {
    // Tiles captured in the next move
    static int lastCapturedCount = 0;
    int currentCaptured = countCapturedTiles();
    int newlyCaptured = currentCaptured - lastCapturedCount;
    lastCapturedCount = currentCaptured;
    return newlyCaptured > 0 ? newlyCaptured : 0;
}

void Game::updatePlayerScore(GamePlayer* player, int tilesCaptured) {
    if (!player) return;

    int points = 0;

    if (tilesCaptured <= 0) {
        return;
    }

    // Base points: 1 point per tile
    points = tilesCaptured;
    int multiplier = 1;

    if (bonusCounter < 3) {
        if (tilesCaptured > 10) {
            multiplier = 2;
            bonusCounter++;
        }
    }
    else if (bonusCounter < 5) {
        if (tilesCaptured > 5) {
            multiplier = 2;
            bonusCounter++;
        }
    }
    else {
        if (tilesCaptured > 5) {
            multiplier = 4;
            bonusCounter++;
        }
    }

    points *= multiplier;
    player->setScore(player->getScore() + points);

    // Check for power-up award
    checkPowerUpAward(player);
}

void Game::calculateMultiplayerScores() {
    if (!player1 || !player2) return;

    int totalCells = 0;
    int p1Territory = 0;
    int p2Territory = 0;
    int sharedTerritory = 0;

    // Count territory for each player
    for (int i = 1; i < grid->getRows() - 1; i++) {
        for (int j = 1; j < grid->getCols() - 1; j++) {
            totalCells++;
            int cell = grid->getCell(i, j);
            if (cell == 2) p1Territory++;      // Player 1's color
            else if (cell == 3) p2Territory++; // Player 2's color  
            else if (cell == 1) sharedTerritory++; // Captured by both
        }
    }

    if (totalCells > 0) {
        // Calculate scores based on territory control
        float p1Percentage = (float)(p1Territory + sharedTerritory / 2) / totalCells;
        float p2Percentage = (float)(p2Territory + sharedTerritory / 2) / totalCells;

        int p1Score = (int)(p1Percentage * 5000);
        int p2Score = (int)(p2Percentage * 5000);

        // Survival bonus
        if (player1->isAlive()) p1Score += 1000;
        if (player2->isAlive()) p2Score += 1000;

        player1->setScore(p1Score);
        player2->setScore(p2Score);

        // Level completion bonus for both players if target reached
        if (p1Percentage >= targetPercentage || p2Percentage >= targetPercentage) {
            player1->setScore(player1->getScore() + 500);
            player2->setScore(player2->getScore() + 500);
        }
    }
}

void Game::capturePlayerArea(int playerNum) {
    int tilesBefore = countCapturedTiles();

    for (int i = 0; i < enemyCount; i++)
        grid->drop(enemies[i].getY() / ts, enemies[i].getX() / ts);

    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getCols(); j++) {
            grid->setCell(i, j, (grid->getCell(i, j) == -1) ? 0 : 1);
        }
    }

    int tilesAfter = countCapturedTiles();
    int tilesCaptured = tilesAfter - tilesBefore;

    if (tilesCaptured > 0) {
        if (playerNum == 1) {
            updatePlayerScore(player1, tilesCaptured);
            cout << "Player 1 captured " << tilesCaptured << " tiles!" << endl;
        }
        else {
            updatePlayerScore(player2, tilesCaptured);
            cout << "Player 2 captured " << tilesCaptured << " tiles!" << endl;
        }
    }
}

bool Game::hasEnclosedArea(int playerNum) {

    int trailCount = 0;
    int trailValue = (playerNum == 1) ? 2 : 3;

    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getCols(); j++) {
            if (grid->getCell(i, j) == trailValue) {
                trailCount++;
            }
        }
    }

    return trailCount >= 4;
}

void Game::updateMultiplayerPlayer(GamePlayer* player, int playerNum, float dt) {
    if (!player) return;

    // Store previous alive state to detect death
    bool wasAlive = player->isAlive();

    int oldX = player->getX();
    int oldY = player->getY();

    player->move(delay, dt);

    // Get new position
    int newX = player->getX();
    int newY = player->getY();

    bool moved = (oldX != newX || oldY != newY);
    if (!moved) return;

    int cellValue = grid->getCell(newY, newX);
    int trailValue = (playerNum == 1) ? 2 : 3;

    // Handle collisions and trail logic
    if (cellValue == trailValue) {
        // Hit own trail
        if (player->isConstructing()) {
            player->setAlive(false);
            cout << "Player " << playerNum << " died - hit own trail!" << endl;
        }
    }
    else if (cellValue == 0) {
        // Empty space - leave trail
        grid->setCell(newY, newX, trailValue);
        player->setConstructing(true);
    }
    else if (cellValue == 1) {
        // Captured area - try to capture
        if (player->isConstructing() && hasEnclosedArea(playerNum)) {
            capturePlayerArea(playerNum);
            player->setConstructing(false);
        }
    }
    else if ((playerNum == 1 && cellValue == 3) || (playerNum == 2 && cellValue == 2)) {
        // Hit other player's trail
        player->setAlive(false);
        cout << "Player " << playerNum << " died - hit opponent's trail!" << endl;
    }

    // Clear trails if player just died
    if (wasAlive && !player->isAlive()) {
        clearPlayerTrails(playerNum);
    }
}

void Game::update(float dt) {
    // Update power-ups for both modes
    if (player1) player1->updatePowerUp(dt);
    if (player2) player2->updatePowerUp(dt);

    // Update freeze timer
    if (enemiesFrozen) {
        enemyFreezeTimer -= dt;
        if (enemyFreezeTimer <= 0) {
            enemiesFrozen = false;
            enemyFreezeTimer = 0;
        }
    }

    if (isMultiplayer) {
        // Move players
        bool p1Frozen = (player2 && player2->hasPowerUp());
        bool p2Frozen = (player1 && player1->hasPowerUp());

        // Update Player 1
        if (player1 && player1->isAlive() && !p1Frozen) {
            updateMultiplayerPlayer(player1, 1, dt);
        }

        // Update Player 2  
        if (player2 && player2->isAlive() && !p2Frozen) {
            updateMultiplayerPlayer(player2, 2, dt);
        }

        // Move enemies if not frozen
        if (!enemiesFrozen) {
            for (int i = 0; i < enemyCount; i++)
                enemies[i].move();
        }

        checkCollisions();
    }
    else {
        // Single player update
        if (timer >= delay) {
            x += dx;
            y += dy;

            if (x < 0) x = 0; if (x > grid->getCols() - 1) x = grid->getCols() - 1;
            if (y < 0) y = 0; if (y > grid->getRows() - 1) y = grid->getRows() - 1;

            if (grid->getCell(y, x) == 2) GameRunning = false;
            if (grid->getCell(y, x) == 0) grid->setCell(y, x, 2);
            timer = 0;
        }

        // Move enemies if not frozen
        if (!enemiesFrozen) {
            for (int i = 0; i < enemyCount; i++)
                enemies[i].move();
        }

        if (grid->getCell(y, x) == 1) {
            dx = dy = 0;

            // Capture area and calculate score
            int tilesBefore = countCapturedTiles();

            for (int i = 0; i < enemyCount; i++)
                grid->drop(enemies[i].getY() / ts, enemies[i].getX() / ts);

            for (int i = 0; i < grid->getRows(); i++)
                for (int j = 0; j < grid->getCols(); j++)
                    grid->setCell(i, j, (grid->getCell(i, j) == -1) ? 0 : 1);

            int tilesAfter = countCapturedTiles();
            int tilesCaptured = tilesAfter - tilesBefore;
            addScore(tilesCaptured);
        }

        checkCollisions();
    }
}

MenuOptions Game::run() {
    Clock clock;

    while (window->isOpen() && GameRunning) {
        float dt = clock.restart().asSeconds();
        timer += dt;

        processInput();
        if (GameRunning) {
            update(dt);
        }
        render();
    }

    sleep(seconds(2));

    if (isMultiplayer) {
        calculateMultiplayerScores();
        
        // Update leaderboard for both players
        if (!currentUsername.empty() && currentPlayerID != 0) {

            leaderboard->addOrUpdate(currentUsername, player1->getScore(), currentPlayerID);
        }
        if (!player2Username.empty() && player2PlayerID != 0) {
            leaderboard->addOrUpdate(player2Username, player2->getScore(), player2PlayerID);
        }
    } else {
        if (player1) player1->setScore(score);
        
        // Update leaderboard with current player's score
        if (!currentUsername.empty() && currentPlayerID != 0) {
            leaderboard->addOrUpdate(currentUsername, score, currentPlayerID);
        }
    }

    Player* curr = gameManager->getCurrentsPlayer();
    curr->totalGames++;
    curr->totalPoints += player1->getScore();
    //gameManager->updatePlayerStats(player1->getScore());

    return ENDMENU;
}

void Game::processInput() {
    Event e;
    while (window->pollEvent(e)) {
        if (e.type == Event::Closed)
            window->close();

        if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {
            grid->clear();
            x = 10; y = 0;
            GameRunning = true;
        }

        // Power Up Activations
        if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space) {
            if (player1 && player1->getPowerUpCount() > 0 && !player1->hasPowerUp()) {
                player1->activatePowerUp();
                enemiesFrozen = true;
                enemyFreezeTimer = 3.0f;
                player1->incrementPowerUpCount(-1);
                cout << "Power-up activated! Enemies frozen for 3 seconds." << endl;
            }
        }

        if (e.type == Event::KeyPressed && e.key.code == Keyboard::O && isMultiplayer) {
            if (player2 && player2->getPowerUpCount() > 0 && !player2->hasPowerUp()) {
                player2->activatePowerUp();
                enemiesFrozen = true;
                enemyFreezeTimer = 3.0f;
                player2->incrementPowerUpCount(-1);
                cout << "Player 2 power-up activated!" << endl;
            }
        }
    }

    if (isMultiplayer) {
        // Player 1 Controls - WASD
        if (Keyboard::isKeyPressed(Keyboard::A))
            player1->setDirection(-1, 0);
        if (Keyboard::isKeyPressed(Keyboard::D))
            player1->setDirection(1, 0);
        if (Keyboard::isKeyPressed(Keyboard::W))
            player1->setDirection(0, -1);
        if (Keyboard::isKeyPressed(Keyboard::S))
            player1->setDirection(0, 1);

        // Player 2 Controls - Arrow keys
        if (Keyboard::isKeyPressed(Keyboard::Left))
            player2->setDirection(-1, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right))
            player2->setDirection(1, 0);
        if (Keyboard::isKeyPressed(Keyboard::Up))
            player2->setDirection(0, -1);
        if (Keyboard::isKeyPressed(Keyboard::Down))
            player2->setDirection(0, 1);
    }
    else {
        // Single player inputs
        if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; }
        if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1;  dy = 0; }
        if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0;  dy = -1; }
        if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0;  dy = 1; }
    }
    // Save Game
    if (Keyboard::isKeyPressed(Keyboard::G)) {
        saveGame();
    }
}

void Game::checkPlayerCollisions() {
    if (!player1 || !player2) return;
    if (!player1->isAlive() || !player2->isAlive()) return;
    
    // Check if players are at same position
    if (player1->getX() == player2->getX() && player1->getY() == player2->getY()) {
        // Both constructing - both die
        if (player1->isConstructing() && player2->isConstructing()) {
            player1->setAlive(false);
            player2->setAlive(false);
            cout << "Both players died - collision while constructing!" << endl;
        }
        // Player 1 constructing, Player 2 not - Player 1 dies
        else if (player1->isConstructing() && !player2->isConstructing()) {
            player1->setAlive(false);
            cout << "Player 1 died - collided with Player 2!" << endl;
        }
        // Player 2 constructing, Player 1 not - Player 2 dies
        else if (player2->isConstructing() && !player1->isConstructing()) {
            player2->setAlive(false);
            cout << "Player 2 died - collided with Player 1!" << endl;
        }
    }
    
    // Check if Player 1 touches Player 2's trail
    if (player1->isAlive()) {
        int cellValue = grid->getCell(player1->getY(), player1->getX());
        if (cellValue == 3) {  // Player 2's trail
            player1->setAlive(false);
            cout << "Player 1 died - touched Player 2's trail!" << endl;
        }
    }
    
    // Check if Player 2 touches Player 1's trail
    if (player2->isAlive()) {
        int cellValue = grid->getCell(player2->getY(), player2->getX());
        if (cellValue == 2) {  // Player 1's trail
            player2->setAlive(false);
            cout << "Player 2 died - touched Player 1's trail!" << endl;
        }
    }
}

void Game::checkCollisions() {
    if (isMultiplayer) {
        // Check player collisions
        checkPlayerCollisions();

        // Check if both players are dead
        if (player1 && player2 && !player1->isAlive() && !player2->isAlive()) {
            GameRunning = false;
        }

        // Check enemy collisions for both players - FIXED LOGIC
        bool p1Frozen = (player2 && player2->hasPowerUp());
        bool p2Frozen = (player1 && player1->hasPowerUp());

        if (!p1Frozen && player1 && player1->isAlive()) {
            for (int i = 0; i < enemyCount; i++) {
                int enemyGridX = enemies[i].getX() / ts;
                int enemyGridY = enemies[i].getY() / ts;

                // Check if player is on the same cell as enemy
                if (player1->getX() == enemyGridX && player1->getY() == enemyGridY) {
                    player1->setAlive(false);
                    cout << "Player 1 killed by enemy at position!" << endl;
                }
                // Check if enemy is on player's trail
                else if (grid->getCell(enemyGridY, enemyGridX) == 2) {
                    // Enemy touched player 1's trail - kill player 1
                    player1->setAlive(false);
                    cout << "Player 1 killed - enemy touched trail!" << endl;
                }
            }
        }

        if (!p2Frozen && player2 && player2->isAlive()) {
            for (int i = 0; i < enemyCount; i++) {
                int enemyGridX = enemies[i].getX() / ts;
                int enemyGridY = enemies[i].getY() / ts;

                // Check if player is on the same cell as enemy
                if (player2->getX() == enemyGridX && player2->getY() == enemyGridY) {
                    player2->setAlive(false);
                    cout << "Player 2 killed by enemy at position!" << endl;
                }
                // Check if enemy is on player's trail
                else if (grid->getCell(enemyGridY, enemyGridX) == 3) {
                    // Enemy touched player 2's trail - kill player 2
                    player2->setAlive(false);
                    cout << "Player 2 killed - enemy touched trail!" << endl;
                }
            }
        }
    }
    else {
        // Single player collision check
        for (int i = 0; i < enemyCount; i++) {
            int enemyGridX = enemies[i].getX() / ts;
            int enemyGridY = enemies[i].getY() / ts;

            // Player position collision
            if (x == enemyGridX && y == enemyGridY) {
                GameRunning = false;
                cout << "Player killed by enemy!" << endl;
            }
            // Enemy on trail collision
            else if (grid->getCell(enemyGridY, enemyGridX) == 2) {
                GameRunning = false;
                cout << "Player killed - enemy touched trail!" << endl;
            }
        }
    }
}

void Game::clearPlayerTrails(int playerNum) {
    int trailValue = (playerNum == 1) ? 2 : 3;

    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getCols(); j++) {
            if (grid->getCell(i, j) == trailValue) {
                grid->setCell(i, j, 0); // Clear the trail
            }
        }
    }
    cout << "Cleared Player " << playerNum << " trails" << endl;
}

void Game::render() {
    window->clear(bgColor);

    // Draw grid background 
    RectangleShape background(Vector2f(N * ts, M * ts));
    background.setFillColor(Color(30, 30, 30, 100)); // Semi-dark background
    window->draw(background);

    // Draw grid tiles
    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getCols(); j++) {
            int cell = grid->getCell(i, j);
            if (cell == 0) continue;

            if (cell == 1) sTile.setTextureRect({ 0, 0, ts, ts });        // Captured area
            else if (cell == 2) sTile.setTextureRect({ 54, 0, ts, ts });  // Player 1 trail (red)
            else if (cell == 3) sTile.setTextureRect({ 72, 0, ts, ts });  // Player 2 trail (blue)

            sTile.setPosition(j * ts, i * ts);
            window->draw(sTile);
        }
    }

    // Draw players
    if (isMultiplayer) {
        // Draw Player 1 with glow effect when alive
        if (player1 && player1->isAlive()) {
            sTile.setTextureRect({ 36, 0, ts, ts });
            sTile.setPosition(player1->getX() * ts, player1->getY() * ts);

            // Add subtle glow for active player
            if (player1->isConstructing()) {
                CircleShape glow(ts / 2);
                glow.setPosition(player1->getX() * ts, player1->getY() * ts);
                glow.setFillColor(Color(255, 0, 0, 50));
                window->draw(glow);
            }
            window->draw(sTile);
        }

        // Draw Player 2 with glow effect when alive
        if (player2 && player2->isAlive()) {
            sTile.setTextureRect({ 18, 0, ts, ts });
            sTile.setPosition(player2->getX() * ts, player2->getY() * ts);

            // Add subtle glow for active player
            if (player2->isConstructing()) {
                CircleShape glow(ts / 2);
                glow.setPosition(player2->getX() * ts, player2->getY() * ts);
                glow.setFillColor(Color(0, 0, 255, 50));
                window->draw(glow);
            }
            window->draw(sTile);
        }
    }
    else {
        // Draw single player with glow effect
        sTile.setTextureRect({ 36, 0, ts, ts });
        sTile.setPosition(x * ts, y * ts);

        // Add subtle glow when constructing
        if (grid->getCell(y, x) == 0) {
            CircleShape glow(ts / 2);
            glow.setPosition(x * ts, y * ts);
            glow.setFillColor(Color(255, 255, 0, 50));
            window->draw(glow);
        }
        window->draw(sTile);
    }

    // Draw Enemies with rotation
    sEnemy.rotate(5); // Slower rotation for better visual
    for (int i = 0; i < enemyCount; i++) {
        sEnemy.setPosition(enemies[i].getX(), enemies[i].getY());

        // Add frozen effect
        if (enemiesFrozen) {
            sEnemy.setColor(Color(100, 100, 255)); // Blue tint when frozen
        }
        else {
            sEnemy.setColor(Color::White);
        }

        window->draw(sEnemy);
    }

    // UI Background for better readability
    RectangleShape uiBackground(Vector2f((N * ts)/2 - 100, 180));
    uiBackground.setFillColor(Color(0, 0, 0, 160)); // Semi-transparent black
    uiBackground.setPosition(0, 0);
    window->draw(uiBackground);

    // Draw UI
    Font font;
    if (font.loadFromFile("assets/monogram.ttf")) {
        if (isMultiplayer) {
            drawMultiplayerUI(font);
        }
        else {
            drawSinglePlayerUI(font);
        }

        // Draw game over screen if game ended
        if (!GameRunning) {
            drawGameOverScreen(font);
        }
    }

    window->display();
}

void Game::drawMultiplayerUI(const Font& font) {
    const float leftMargin = 15.0f;
    const float rightMargin = N * ts - 165.0f;
    const float lineHeight = 25.0f;
    float currentY = 15.0f;

    // Player 1 Info (Left Side)
    Text p1Name("P1: " + currentUsername, font, 16);
    p1Name.setPosition(leftMargin, currentY);
    p1Name.setFillColor(Color::Red);
    p1Name.setStyle(Text::Bold);
    window->draw(p1Name);
    currentY += lineHeight;

    Text p1Score("Score: " + to_string(player1 ? player1->getScore() : 0), font, 16);
    p1Score.setPosition(leftMargin, currentY);
    p1Score.setFillColor(Color::White);
    window->draw(p1Score);
    currentY += lineHeight;

    Text p1PowerUps("Power-ups: " + to_string(player1 ? player1->getPowerUpCount() : 0), font, 14);
    p1PowerUps.setPosition(leftMargin, currentY);
    p1PowerUps.setFillColor(Color::Yellow);
    window->draw(p1PowerUps);
    currentY += lineHeight;

    if (player1 && player1->hasPowerUp()) {
        Text p1Active("Active: " + to_string((int)player1->getPowerUpTimer()) + "s", font, 12);
        p1Active.setPosition(leftMargin, currentY);
        p1Active.setFillColor(Color::Green);
        window->draw(p1Active);
        currentY += lineHeight - 5;
    }

    Text p1Controls("Controls: WASD | P=Power", font, 12);
    p1Controls.setPosition(leftMargin, currentY);
    p1Controls.setFillColor(Color::Cyan);
    window->draw(p1Controls);

    // Player 2 Info (Right Side)
    currentY = 15.0f;

    Text p2Name("P2: " + player2Username, font, 16);
    p2Name.setPosition(rightMargin, currentY);
    p2Name.setFillColor(Color::Blue);
    p2Name.setStyle(Text::Bold);
    window->draw(p2Name);
    currentY += lineHeight;

    Text p2Score("Score: " + to_string(player2 ? player2->getScore() : 0), font, 16);
    p2Score.setPosition(rightMargin, currentY);
    p2Score.setFillColor(Color::White);
    window->draw(p2Score);
    currentY += lineHeight;

    Text p2PowerUps("Power-ups: " + to_string(player2 ? player2->getPowerUpCount() : 0), font, 14);
    p2PowerUps.setPosition(rightMargin, currentY);
    p2PowerUps.setFillColor(Color::Yellow);
    window->draw(p2PowerUps);
    currentY += lineHeight;

    if (player2 && player2->hasPowerUp()) {
        Text p2Active("Active: " + to_string((int)player2->getPowerUpTimer()) + "s", font, 12);
        p2Active.setPosition(rightMargin, currentY);
        p2Active.setFillColor(Color::Green);
        window->draw(p2Active);
        currentY += lineHeight - 5;
    }

    Text p2Controls("Controls: Arrows | O=Power", font, 12);
    p2Controls.setPosition(rightMargin, currentY);
    p2Controls.setFillColor(Color::Cyan);
    window->draw(p2Controls);

    // Center Info
    string levelStr = (currentLevel == EASY) ? "EASY" :
        (currentLevel == MEDIUM) ? "MEDIUM" : "HARD";
    Text levelText("Level: " + levelStr, font, 14);
    levelText.setPosition(N * ts / 2 - 40, 15);
    levelText.setFillColor(Color::Magenta);
    window->draw(levelText);
}

void Game::drawSinglePlayerUI(const Font& font) {
    const float leftMargin = 15.0f;
    const float lineHeight = 22.0f;
    float currentY = 15.0f;

    // Player Info
    if (!currentUsername.empty()) {
        Text playerText("Player: " + currentUsername, font, 16);
        playerText.setPosition(leftMargin, currentY);
        playerText.setFillColor(Color::Yellow);
        playerText.setStyle(Text::Bold);
        window->draw(playerText);
        currentY += lineHeight;
    }

    // Score
    Text scoreText("Score: " + to_string(score), font, 18);
    scoreText.setPosition(leftMargin, currentY);
    scoreText.setFillColor(Color::White);
    scoreText.setStyle(Text::Bold);
    window->draw(scoreText);
    currentY += lineHeight + 2;

    // Power-ups
    Text powerUpText("Power-ups: " + to_string(player1->getPowerUpCount()), font, 16);
    powerUpText.setPosition(leftMargin, currentY);
    powerUpText.setFillColor(Color::Yellow);
    window->draw(powerUpText);
    currentY += lineHeight;

    if (player1 && player1->hasPowerUp()) {
        Text activePower("Active: " + to_string((int)player1->getPowerUpTimer()) + "s", font, 14);
        activePower.setPosition(leftMargin, currentY);
        activePower.setFillColor(Color::Green);
        window->draw(activePower);
        currentY += lineHeight - 3;
    }

    // Game Info
    string levelStr = (currentLevel == EASY) ? "EASY" :
        (currentLevel == MEDIUM) ? "MEDIUM" : "HARD";
    Text levelText("Level: " + levelStr, font, 14);
    levelText.setPosition(leftMargin, currentY);
    levelText.setFillColor(Color::Cyan);
    window->draw(levelText);
    currentY += lineHeight;

    // Bonus System
    Text bonusText("Bonus: " + to_string(bonusCounter), font, 12);
    bonusText.setPosition(leftMargin, currentY);
    bonusText.setFillColor(Color(255, 165, 0)); // Orange
    window->draw(bonusText);
    currentY += lineHeight;

    // Controls hint
    Text controls("Space: Power-up | S: Save", font, 11);
    controls.setPosition(leftMargin, currentY);
    controls.setFillColor(Color(200, 200, 200));
    window->draw(controls);
}

void Game::drawGameOverScreen(const Font& font) {
    // Semi-transparent overlay
    RectangleShape overlay(Vector2f(N * ts, M * ts));
    overlay.setFillColor(Color(0, 0, 0, 180));
    window->draw(overlay);

    // Game over sprite (centered)
    sGameover.setPosition(N * ts / 2 - sGameover.getLocalBounds().width / 2,
                          M * ts / 2 - sGameover.getLocalBounds().height / 2 - 50);
    window->draw(sGameover);

    if (isMultiplayer) {
        int p1Score = player1 ? player1->getScore() : 0;
        int p2Score = player2 ? player2->getScore() : 0;

        // Result text
        Text result;
        result.setFont(font);
        result.setCharacterSize(36);
        result.setStyle(Text::Bold);
        result.setPosition(N * ts / 2 - 150, M * ts / 2 + 30);

        if (p1Score > p2Score) {
            result.setString("PLAYER 1 WINS!");
            result.setFillColor(Color::Red);
        }
        else if (p2Score > p1Score) {
            result.setString("PLAYER 2 WINS!");
            result.setFillColor(Color::Blue);
        }
        else {
            result.setString("TIE GAME!");
            result.setFillColor(Color::Yellow);
        }

        // Center the result text
        FloatRect resultBounds = result.getLocalBounds();
        result.setPosition(N * ts / 2 - resultBounds.width / 2, M * ts / 2 + 30);
        window->draw(result);

        // Scores
        Text scores;
        scores.setFont(font);
        scores.setString("P1: " + to_string(p1Score) + "  |  P2: " + to_string(p2Score));
        scores.setCharacterSize(24);
        scores.setFillColor(Color::White);
        FloatRect scoreBounds = scores.getLocalBounds();
        scores.setPosition(N * ts / 2 - scoreBounds.width / 2, M * ts / 2 + 80);
        window->draw(scores);

    }
    else {
        // Single player final score
        Text finalScore("FINAL SCORE: " + to_string(score), font, 32);
        finalScore.setFillColor(Color::Green);
        finalScore.setStyle(Text::Bold);
        FloatRect scoreBounds = finalScore.getLocalBounds();
        finalScore.setPosition(N * ts / 2 - scoreBounds.width / 2, M * ts / 2 + 50);
        window->draw(finalScore);

        // Level completion status
        float filledPercentage = (float)countCapturedTiles() / (grid->getRows() * grid->getCols()) * 100;
        Text completion("Map Completion: " + to_string((int)filledPercentage) + "%", font, 20);
        completion.setFillColor(Color::Cyan);
        FloatRect compBounds = completion.getLocalBounds();
        completion.setPosition(N * ts / 2 - compBounds.width / 2, M * ts / 2 + 90);
        window->draw(completion);
    }

}

void Game::saveGame() {
    GameState state;
    state.score = score;
    state.lives = 1; // Default to 1 life as game ends on hit
    state.level = (int)currentLevel;
    state.playerX = x;
    state.playerY = y;
    
    // Add filled tiles to save manager
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid->getCell(i, j) == 1) { // Use grid->getCell
                saveManager.addTile(i, j);
            }
        }
    }
    
    if (saveManager.saveGame(state, currentUsername)) {
        cout << "Game Saved Successfully!" << endl;
    } else {
        cout << "Failed to save game!" << endl;
    }
}

void Game::loadGame() {
    GameState state;
    string savedUser;
    
    if (saveManager.loadGame(state, savedUser)) {
        // Only load if username matches or if it's a single player generic save
        if (savedUser == currentUsername || currentUsername.empty()) {
            score = state.score;
            // lives = state.lives; // Ignore lives
            currentLevel = (GameLevel)state.level;
            x = state.playerX;
            y = state.playerY;
            
            // Reset grid
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    grid->setCell(i, j, 0);
                }
            }
            
            // Load tiles
            TileNode* tiles = saveManager.getTiles();
            while (tiles != nullptr) {
                if (tiles->x >= 0 && tiles->x < M && tiles->y >= 0 && tiles->y < N) {
                    grid->setCell(tiles->x, tiles->y, 1);
                }
                tiles = tiles->next;
            }
            
            cout << "Game Loaded Successfully!" << endl;
        } else {
            cout << "Save file belongs to another user!" << endl;
        }
    } else {
        cout << "Failed to load game!" << endl;
    }
}