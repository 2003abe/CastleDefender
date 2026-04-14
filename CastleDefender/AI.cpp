#include "AI.h"
#include <cstdlib>
#include <ctime>
#include <algorithm> 

//constructor, first wave, base health, one death counter per column
AI::AI()
    : waveNumber(1), enemyBaseHealth(3)
{
    //learning memory
    deathCount.resize(20, 0);

    //random generator
    std::srand(std::time(nullptr));
}

int AI::getWave() const {
    return waveNumber;
}

int AI::getEnemyBaseHealth() const {
    return enemyBaseHealth;
}

//increases wave number if player performs well
void AI::nextWave(int playerScore, int enemiesPerWave) {
    waveNumber++;

    //adaptive difficulty at 80% success = stronger enemies
    int threshold = static_cast<int>(0.8 * waveNumber * enemiesPerWave * 10);

    if (playerScore >= threshold) {
        enemyBaseHealth++;
    }
}

//used to avoid dangerous spawn columns
void AI::recordDeath(int col) {
    if (col >= 0 && col < (int)deathCount.size()) {
        deathCount[col]++;
    }
}

//selects safe columns
//returns polymorphic Enemy* dynamically allocated (fast/strong/regular)
//output, new Enemy at (r=0,c)
Enemy* AI::spawnEnemy(int gridCols, const std::vector<Tower>& towers)
{
    int topRow = 0;

    int chosenCol = chooseSpawnColumn();

    int roll = std::rand() % 100;

    if (roll < 20) {
        return new FastEnemy(topRow, chosenCol);      
    }
    else if (roll < 50) {
        return new StrongEnemy(topRow, chosenCol);   
    }
    else {
        return new Enemy(topRow, chosenCol);       
    }
}

//weights columns by safety
//output, col index selected by weighted randomness
int AI::chooseSpawnColumn()
{
    const int maxDeath = *std::max_element(deathCount.begin(), deathCount.end());

    // Build weights: safer columns get HIGHER weight.
    std::vector<int> weights(deathCount.size());

    for (int c = 0; c < deathCount.size(); c++) {
        int danger = deathCount[c];
        weights[c] = (maxDeath - danger + 1);
    }

    //compute total weight
    int totalWeight = 0;
    for (int w : weights) totalWeight += w;

    //weighted random selection
    int roll = rand() % totalWeight;

    for (int c = 0; c < weights.size(); c++) {
        roll -= weights[c];
        if (roll <= 0)
            return c;
    }

    return rand() % deathCount.size(); //fallback safety
}

