#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <iomanip>
#include <cmath>


//sets console colour with a code
void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

//rendering symbols for objects
const char EMPTY_CELL = '.';
const char CASTLE_CELL = 'C';
const char TOWER_CELL = 'T';
const char ENEMY_NORMAL_CELL = 'E';
const char ENEMY_FAST_CELL = 'F';
const char ENEMY_STRONG_CELL = 'S';

//constructor, initialises grid, castle, score, kill count and AI
//links splash effect to tower attacks
Game::Game()
    : grid(20, 20), castle(19, 10), score(0), enemiesDestroyed(0)
{
    grid.setSplashColorSource(&splashColorCells);
    srand((unsigned)time(nullptr));
}

//for gameplay refresh between frames
void Game::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//grid initialisation
//prints empty grid with castle at bottom center
void Game::setup() {
    // Reset grid
    for (int r = 0; r < grid.getRows(); r++) {
        for (int c = 0; c < grid.getCols(); c++) {
            grid.setCell(r, c, EMPTY_CELL);
        }
    }

    grid.setCell(castle.getRow(), castle.getCol(), CASTLE_CELL);
}

//checks tower placement using grid coordinates (r, c)
//output is true when valid
bool Game::isValidTowerPosition(int r, int c) {
    if (r < 0 || r >= grid.getRows() || c < 0 || c >= grid.getCols())
        return false;

    //cannot place in top 2 rows
    if (r < 2) return false;

    //cannot place on castle
    if (r == castle.getRow() && c == castle.getCol())
        return false;

    //must be empty
    if (grid.getCell(r, c) != EMPTY_CELL)
        return false;

    return true;
}

//reads 5 tower positions from tower.txt (row, col) pairs
//output is towers[] filled

void Game::placeTowers() {
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    std::cout << "Working directory = " << buffer << "\n";

    std::ifstream fin("towers.txt");
    if (!fin) {
        std::cout << "Could not open towers.txt\n";
        std::cout << "Find towers.txt.\n";
        exit(1);
    }

    int r, c;
    int towerCount = 0;

    std::cout << "reading tower positions (towers.txt)...\n";

    while (towerCount < 5 && fin >> r >> c) {
        if (isValidTowerPosition(r, c)) {
            towers.push_back(Tower(r, c));
            grid.setCell(r, c, 'T');
            towerCount++;
        }
        else {
            std::cout << "Invalid tower position in file: ("
                << r << ", " << c << ")\n";
        }
    }

    fin.close();

    std::cout << "Loaded " << towerCount << " towers.\n";
}


//displays elimination log on the right using messageLog vector
//output, coloured text feed
void Game::printMessageLog()
{
    const int maxLines = 10;
    const int feedX = 60;
    const int feedY = 12;

    //title, KILLCAM
    COORD titlePos = { (SHORT)feedX, (SHORT)(feedY - 1) };
    SetConsoleCursorPosition(hConsole, titlePos);

    setColor(COLOR_CYAN);               
    std::cout << "K I L L C A M";
    setColor(COLOR_DEFAULT);

    int total = static_cast<int>(messageLog.size());
    int startIndex = (total > maxLines) ? (total - maxLines) : 0;

    //clear old feed
    for (int i = 0; i < maxLines; i++) {
        COORD pos = { 0, 0 };
        pos.X = static_cast<SHORT>(feedX);
        pos.Y = static_cast<SHORT>(feedY + i);
        SetConsoleCursorPosition(hConsole, pos);
        std::cout << std::string(50, ' ');
    }

    //print feed
    int line = 0;
    for (int i = startIndex; i < total; i++) {

        COORD pos = { 0, 0 }; 
        pos.X = static_cast<SHORT>(feedX);
        pos.Y = static_cast<SHORT>(feedY + line);
        SetConsoleCursorPosition(hConsole, pos);

        const std::string& msg = messageLog[i];

        //messages in colour
        if (msg.find("sniped") != std::string::npos ||
            msg.find("NO-SCOPED") != std::string::npos)
            setColor(COLOR_GREEN);

        else if (msg.find("BANG") != std::string::npos ||
            msg.find("BOOM") != std::string::npos ||
            msg.find("sploded") != std::string::npos)
            setColor(COLOR_RED);

        else if (msg.find("[UPGRADE]") != std::string::npos)
            setColor(COLOR_CYAN);

        else if (msg.find("Splash damage") != std::string::npos)
            setColor(COLOR_DEFAULT);

        else
            setColor(COLOR_DEFAULT);

        std::cout << msg;
        line++;
    }

    setColor(COLOR_DEFAULT);
}


//asks AI to spawn new enemy types in valid columns
//output, new Enemy* is added to enemies[], stats updated
void Game::spawnEnemy() {
    Enemy* e = ai.spawnEnemy(grid.getCols(), towers);
    enemies.push_back(e);

    //spawns tracked
    if (dynamic_cast<StrongEnemy*>(e))      strongSpawned++;
    else if (dynamic_cast<FastEnemy*>(e))   fastSpawned++;
    else                                    normalSpawned++;
}

//all enemies move 1 step toward the castle
//downward priority, diagonal left/right if blocked
void Game::moveEnemies() {
    //clear old positions
    for (Enemy* e : enemies) {
        if (!e->isDead()) {
            grid.setCell(e->getRow(), e->getCol(), EMPTY_CELL);
        }
    }

    //move enemy
    for (Enemy* e : enemies) {
        if (e->isDead()) continue;

        int r = e->getRow();
        int c = e->getCol();
        int nextR = r + e->getSpeed();

        //land at castle
        if (nextR >= castle.getRow()) {

            //track successful enemy
            if (dynamic_cast<StrongEnemy*>(e))      strongReached++;
            else if (dynamic_cast<FastEnemy*>(e))   fastReached++;
            else                                    normalReached++;

            castle.damage(10);
            e->damage(9999);
            continue;
        }

        //down
        if (grid.getCell(nextR, c) == EMPTY_CELL) {
            e->setRow(nextR);
            continue;
        }

        //down-left
        if (c > 0 && grid.getCell(nextR, c - 1) == EMPTY_CELL) {
            e->setRow(nextR);
            e->setCol(c - 1);
            continue;
        }

        //down-right
        if (c < grid.getCols() - 1 && grid.getCell(nextR, c + 1) == EMPTY_CELL) {
            e->setRow(nextR);
            e->setCol(c + 1);
            continue;
        }
    }
}

//computes tower range
int getDistance(int r1, int c1, int r2, int c2) {
    int dr = r1 - r2;
    int dc = c1 - c2;
    return static_cast<int>(sqrt(dr * dr + dc * dc));
}

//kill messages for log
//just for visual feedback of activity
std::string getKillMessage(const Tower& t, Enemy* e)
{
    int dist = getDistance(t.getRow(), t.getCol(), e->getRow(), e->getCol());

    static std::vector<std::string> weapons = {
        "a Rifle", "an SMG", "a Minigun", "an LMG",
        "a Turret", "a Bow", "a Biplane Gun"
    };

    std::string weapon = weapons[rand() % weapons.size()];
    bool isStrong = dynamic_cast<StrongEnemy*>(e) != nullptr;

    std::vector<std::string> msgs;

    //long-range kills
    msgs.push_back("NO-SCOPED an enemy (" + std::to_string(dist) + "m)");
    msgs.push_back("sniped an enemy (" + std::to_string(dist) + "m)");

    //weapon kills
    msgs.push_back("eliminated an enemy with " + weapon);

    //explosive kills
    msgs.push_back("sploded an enemy!");
    msgs.push_back("made an enemy go out with a BANG!");
    msgs.push_back("made an enemy go out with a BOOM!");

    //strong enemy special
    if (isStrong) {
        msgs.push_back("FINALLY eliminated a StrongEnemy!");
    }

    return msgs[rand() % msgs.size()];
}

//returns tower colour based on level
int getTowerColor(const Tower& t)
{
    int lvl = t.getLevel();

    if (lvl == 1) return COLOR_YELLOW;
    if (lvl == 2) return COLOR_GREEN;
    if (lvl == 3) return COLOR_CYAN;
    if (lvl == 4) return COLOR_MAGENTA;
    if (lvl == 5) return COLOR_RED;
    if (lvl == 6) return COLOR_GREEN;
    if (lvl == 7) return COLOR_CYAN;
    if (lvl == 8) return COLOR_YELLOW;
    if (lvl == 9) return COLOR_MAGENTA;
    if (lvl == 10) return COLOR_RED;

    return COLOR_DEFAULT;
}

//returns special message when StrongEnemy takes damage
//but has not been eliminated yet
inline std::string getStrongHitMessage() {
    return "knocked a StrongEnemy!";
}


//towers scan for enemies in range, apply regular damage + splash
//input, active enemies
//output, updated enemy health, feed messages, splash damage colours
void Game::towersAttack()
{
    splashCells.clear();          //reset splash animation positions
    splashColorCells.clear();     //reset splash colour list

    for (Tower& t : towers)
    {
        for (Enemy* e : enemies)
        {
            if (!e || e->isDead()) continue;

            //target in range...
            if (!t.isInRange(e->getRow(), e->getCol()))
                continue;

            int pr = e->getRow();
            int pc = e->getCol();

            //splash animation
            int tColor = getTowerColor(t); 

            for (int dr = -1; dr <= 1; dr++)
            {
                for (int dc = -1; dc <= 1; dc++)
                {
                    int sr = pr + dr;
                    int sc = pc + dc;

                    if (sr >= 0 && sr < grid.getRows() &&
                        sc >= 0 && sc < grid.getCols())
                    {
                        splashCells.push_back({ sr, sc });
                        splashColorCells.push_back({ sr, sc, tColor });
                    }
                }
            }

            //tower level before attack
            int oldLevel = t.getLevel();

            //attack
            int splashHits = t.attackEnemy(e, enemies);

            //log for splash effect coordinates
            if (splashHits > 1)
            {
                messageLog.push_back(
                    "Splash damage! Tower (" +
                    std::to_string(t.getRow()) + "," +
                    std::to_string(t.getCol()) +
                    ") hit " + std::to_string(splashHits - 1) +
                    " extra enemies!"
                );
            }

            //StrongEnemy non-lethal hits
            if (!e->isDead())
            {
                if (e->getTypeName() == "StrongEnemy")
                {
                    messageLog.push_back(
                        "Tower (" +
                        std::to_string(t.getRow()) + "," +
                        std::to_string(t.getCol()) +
                        ") knocked StrongEnemy[" +
                        std::to_string(e->getID()) + "]!"
                    );
                }
            }

            //kill messages
            if (e->isDead())
            {
                std::string towerLabel = "Tower (" +
                    std::to_string(t.getRow()) + "," +
                    std::to_string(t.getCol()) + ")";

                std::string enemyLabel = e->getTypeName() + "[" +
                    std::to_string(e->getID()) + "]";

                //distance from tower to enemy
                int dist = getDistance(
                    t.getRow(), t.getCol(),
                    e->getRow(), e->getCol()
                );

                //tower weapons
                static std::vector<std::string> weapons = {
                    "a Rifle", "an SMG", "a Minigun", "an LMG",
                    "a Turret", "a Bow", "a Biplane Gun"
                };

                int roll = std::rand() % 5;  //random assignment
                std::string msg;

                if (roll == 0)
                {
                    msg = towerLabel + " NO-SCOPED " +
                        enemyLabel + " (" +
                        std::to_string(dist) + "m)";
                }
                else if (roll == 1)
                {
                    msg = towerLabel + " sniped " +
                        enemyLabel + " (" +
                        std::to_string(dist) + "m)";
                }
                else if (roll == 2)
                {
                    if (e->getTypeName() == "StrongEnemy")
                    {
                        msg = towerLabel + " finally eliminated " +
                            enemyLabel + "!";
                    }
                    else
                    {
                        msg = towerLabel + " sploded " + enemyLabel + "!";
                    }
                }
                else if (roll == 3)
                {
                    msg = towerLabel + " made " +
                        enemyLabel + " go out with a BANG!";
                }
                else
                {
                    std::string weapon = weapons[std::rand() % weapons.size()];
                    msg = towerLabel + " eliminated " +
                        enemyLabel + " with " + weapon;
                }

                messageLog.push_back(msg);
            }

            //level up
            int newLevel = t.getLevel();
            if (newLevel > oldLevel)
            {
                messageLog.push_back(
                    "[UPGRADE] Tower (" +
                    std::to_string(t.getRow()) + "," +
                    std::to_string(t.getCol()) +
                    ") reached Level " +
                    std::to_string(newLevel) + "!"
                );
            }
        }
    }
}


//removes eliminated enemies from the list
//updates kill count, score, AI death column data
//output, enemies[] shrinks and score increases
void Game::cleanupDeadEnemies()
{
    for (int i = enemies.size() - 1; i >= 0; i--) {

        if (enemies[i]->isDead()) {

            //track kills by type.
            Enemy* e = enemies[i];

            if (dynamic_cast<StrongEnemy*>(e))      strongKilled++;
            else if (dynamic_cast<FastEnemy*>(e))   fastKilled++;
            else                                    normalKilled++;

            //AI record and scoring
            int col = e->getCol();
            ai.recordDeath(col);

            score += 10;
            enemiesDestroyed++;

            delete e;
            enemies.erase(enemies.begin() + i);
        }
    }
}


//rebuilds grid
//outputs refreshed board state
void Game::updateGrid()
{
    for (int r = 0; r < grid.getRows(); r++) {
        for (int c = 0; c < grid.getCols(); c++) {
            grid.setCell(r, c, EMPTY_CELL);
        }
    }

    splashColorCells.clear();

    grid.setCell(castle.getRow(), castle.getCol(), CASTLE_CELL);

    for (Tower& t : towers)
    {
        int tr = t.getRow();
        int tc = t.getCol();

        grid.setCell(tr, tc, t.getSymbol());

        splashColorCells.push_back({ tr, tc, t.getColor() });
    }

    for (Enemy* e : enemies)
    {
        if (!e || e->isDead()) continue;
        grid.setCell(e->getRow(), e->getCol(), e->getSymbol());
    }

    for (auto& p : splashCells)
    {
        int r = p.first;
        int c = p.second;

        if (r < 0 || r >= grid.getRows()) continue;
        if (c < 0 || c >= grid.getCols()) continue;

        char existing = grid.getCell(r, c);
        if (existing != EMPTY_CELL) continue;

        grid.setCell(r, c, '*');

        splashColorCells.push_back({ r, c, COLOR_YELLOW });
    }
}

//shows wave progress bar
//input, #, current enemy index, total enemies
void printWaveProgress(int wave, int current, int total) {
    int barWidth = 30;
    float ratio = (float)current / total;
    int filled = (int)(ratio * barWidth);

    int percent = (int)(ratio * 100);

    int color = COLOR_GREEN;
    if (percent > 80) color = COLOR_RED;
    else if (percent > 40) color = COLOR_YELLOW;

    std::cout << "Wave " << wave << " Progress [";

    setColor(color);
    for (int i = 0; i < filled; i++)
        std::cout << "#";

    if (filled < barWidth) {
        std::cout << ">";
        setColor(COLOR_DEFAULT);
        for (int i = filled + 1; i < barWidth; i++)
            std::cout << ".";
    }

    setColor(COLOR_DEFAULT);
    std::cout << "] " << percent << "%\n\n";
}

//runs all 5 waves, enemy spawns, movement, attacks, cleanup
//rendering, kil feed etc
void Game::start() {
    const int waves = 5;
    const int enemiesPerWave = 10;

    clearScreen();
    updateGrid();          // ensure grid is freshly built
    std::cout << "Score: " << score << "\n";
    std::cout << "Castle HP: " << castle.getHealth() << "\n";
    std::cout << "Enemies Alive: " << enemies.size() << "\n\n";

    printWaveProgress(1, 0, enemiesPerWave);   // 0/10 enemies so far
    grid.print();
    printMessageLog();                         // (empty at start)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    for (int wave = 1; wave <= waves; wave++) {

        for (int i = 0; i < enemiesPerWave; i++) {

            spawnEnemy();
            moveEnemies();
            towersAttack();
            cleanupDeadEnemies();
            updateGrid();

            //frame render
            clearScreen();

            std::cout << "Score: " << score << "\n";
            std::cout << "Castle HP: " << castle.getHealth() << "\n";
            std::cout << "Enemies Alive: " << enemies.size() << "\n\n";

            printWaveProgress(wave, i + 1, enemiesPerWave);

            grid.print();

            printMessageLog();
            //end frame

            //lose condition
            if (castle.isDestroyed()) {

                clearScreen();
                std::cout << "GAME OVER\n\n";
                std::cout << "Final Score: " << score << "\n";
                std::cout << "Enemies Destroyed: " << enemiesDestroyed << "\n";
                std::cout << "Castle Health: " << castle.getHealth() << "\n";
                std::cout << "Winner: AI\n\n";

                // Move cursor a bit down so leaderboard doesn’t overwrite title
                SetConsoleCursorPosition(hConsole, { 0, 1 });
                showFinalEnemyLeaderboard();

                return;
            }


            std::this_thread::sleep_for(std::chrono::milliseconds(600));
        }

        //short pause between waves
        std::this_thread::sleep_for(std::chrono::milliseconds(150));

        //adaptive difficulty
        ai.nextWave(score, enemiesPerWave);
    }

    //game over
    //remove kill feed
    messageLog.clear();

    clearScreen();
    SetConsoleCursorPosition(hConsole, { 0, 0 });

    //result summary
    if (!castle.isDestroyed())
    {
        setColor(COLOR_CYAN);
        std::cout << "VICTORY ROYALE!\n\n";
        setColor(COLOR_DEFAULT);

        std::cout << "Final Score: " << score << "\n";
        std::cout << "Enemies Destroyed: " << enemiesDestroyed << "\n";
        std::cout << "Castle Health: " << castle.getHealth() << "\n";
        std::cout << "Winner: Player 1\n\n";
    }
    else
    {
        std::cout << "GAME OVER\n\n";
        std::cout << "Final Score: " << score << "\n";
        std::cout << "Enemies Destroyed: " << enemiesDestroyed << "\n";
        std::cout << "Castle Health: " << castle.getHealth() << "\n";
        std::cout << "Winner: AI\n\n";
    }

    showFinalEnemyLeaderboard();
}

//shows final enemy stats from internal counters for enemy types
void Game::showFinalEnemyLeaderboard()
{
    COORD pos = { 0, 8 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

    std::cout << "Enemy Leaderboard\n\n";

    auto printCard = [&](const std::string& name,
        int spawned, int eliminated, int reached)
        {
            int alive = spawned - eliminated - reached;
            if (alive < 0) alive = 0;

            std::cout << name << "\n";
            std::cout << "  Spawned:               " << spawned << "\n";
            std::cout << "  Eliminations:          " << eliminated << "\n";
            std::cout << "  Landed at Castle:      " << reached << "\n";
            std::cout << "  Alive:                 " << alive << "\n";

            std::cout << "  Landing/Elim Ratio:    ";

            if (eliminated == 0 && reached == 0) {
                std::cout << "0.00\n\n";
            }
            else if (eliminated == 0) {
                std::cout << "INF\n\n";
            }
            else {
                float ratio = reached / static_cast<float>(eliminated);
                std::cout << std::fixed << std::setprecision(2)
                    << ratio << "\n\n";
            }
        };

    printCard("Normal Enemy", normalSpawned, normalKilled, normalReached);
    printCard("Fast Enemy", fastSpawned, fastKilled, fastReached);
    printCard("Strong Enemy", strongSpawned, strongKilled, strongReached);
}




