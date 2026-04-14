#pragma once
#ifndef AI_H
#define AI_H

#include <vector>
#include "Enemy.h"
#include "StrongEnemy.h"
#include "FastEnemy.h"
#include "Tower.h"


//controls enemy spawning, adaptive difficulty
//tracks dangerous columns (those with many enemy deaths)
//increases base enemy health when player performs well
class AI {
private:
    int waveNumber;
    int enemyBaseHealth;
    std::vector<int> deathCount; //learning memory


public:
    //constructor - initialises wave(1), base health(3), death counters
    //per column
    AI();

    int getWave() const;
    int getEnemyBaseHealth() const;

    //inputs, playerScore, enemiesPerWave
    //output, advances to next wave, increase enemyBaseHealth
    //if player achieved >= 80% of max possible score
    void nextWave(int playerScore, int enemiesPerWave);

    //chooses columns based on learned danger
    // returns a polymorphic enemy pointer (Enemy*, FastEnemy*, StrongEnemy*)
    //output, pointer to new enemy
    Enemy* spawnEnemy(int gridCols,
        const std::vector<Tower>& towers);

    //weighted randomness; colums with fewer deaths = more likely
    //output, integer column index
    int chooseSpawnColumn();

    //input, column index where enemy was eliminated
    //updates AI learning memory for spawns
    void recordDeath(int col);
};

#endif


