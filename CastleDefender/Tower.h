#pragma once
#ifndef TOWER_H
#define TOWER_H

#include <vector>
#include "Enemy.h"

//defensive towers placed by the player
//guards the castle from a fixed position, attacks enemies in range
//gain XP from eliminations and uograde
class Tower {
private:
    int row, col;        //tower grid position

    int range;           //max attack radius
    int damage;          //damage to primary target

    int level;           //for upgrades (1-10)
    int xp;              //points to next level

    char symbol;          //character symbol

public:
    static const int MAX_LEVEL = 10;

    //constructor. rng = range, dmg = damage
    Tower(int r, int c, int rng = 2, int dmg = 1);

    //gets colour index based on tower lvl
    int getColor() const;

    //get position
    int getRow() const;
    int getCol() const;  

    //get tower stats
    int getRange() const;
    int getDamage() const;
    int getLevel() const;

    //returns true if enemy is within range
    bool isInRange(int enemyRow, int enemyCol) const;

    //towers gain XP with each elimination
    void gainXP();
    bool isReadyToUpgrade() const;

    //upgrades increase damage/range
    void upgrade();

    //returns displayed symbol
    char getSymbol() const;

    //special attack - splash damage (primary target + enemies within 3x3 area
    //takes primary enemy pointer + list of enemies
    //output - total enemies hit
    int attackEnemy(Enemy* primary, std::vector<Enemy*>& enemies);
};

#endif


