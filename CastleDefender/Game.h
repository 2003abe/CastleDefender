#pragma once
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

#include "Grid.h"
#include "Enemy.h"
#include "StrongEnemy.h"
#include "FastEnemy.h"
#include "Tower.h"
#include "Castle.h"
#include "AI.h"


//controls the whole game - grid, towers, enemies, waves, scores etc
class Game {
private:
    //battlefield 20x20
    Grid grid;                    
    
    //defensive structure at the bottom center
    Castle castle;                                              

    std::vector<Tower> towers;      //player-placed towers
    std::vector<Enemy*> enemies;    //all active enemies

    //visual splash attack efects
    std::vector<std::pair<int, int>> splashCells;
    std::vector<std::tuple<int, int, int>> splashColorCells;

    //elimination feed during gameplay
    std::vector<std::string> messageLog;

    //AI for enemy spawning
    AI ai;                                      

    //player scores +10 per elimination
    int score;

    //total eliminations
    int enemiesDestroyed;

    // enemy statistics
    int normalSpawned = 0;
    int fastSpawned = 0;
    int strongSpawned = 0;

    int normalKilled = 0;
    int fastKilled = 0;
    int strongKilled = 0;

    int normalReached = 0;
    int fastReached = 0;
    int strongReached = 0;


    //visual resetting before each frame
    void clearScreen();

public:
    //constructs new game: fresh grid, castle, AI, score
    Game();

    //clears board, castle setup
    void setup();

    //read tower placements from towers.txt
    //input - (row,col) pairs
    //output - towers vector
    bool isValidTowerPosition(int r, int c);
    void placeTowers();

  

    //AI selects enemy type+spawn column
    void spawnEnemy();

    //enemy movement towards the castle (downward + diagonal)
    void moveEnemies();

    //towers attack enemies in range - primary + splash damage
    void towersAttack();

    //discards enemies KIA, updates score and statistics
    void cleanupDeadEnemies();

    //rebuilds game at each frame
    void updateGrid();

    //displays end game statistics per enemy type
    void showFinalEnemyLeaderboard();

    //prints an update feed during gameplay (KILLCAM)
    void printMessageLog(); 

    //main game loop - waves, frames, wins/loss, final summary, leaderboard
    void start();
};

#endif
