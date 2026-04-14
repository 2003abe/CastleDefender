#include "Tower.h"
#include <cmath>
#include <iostream>

//constructor for tower stats, position initialisation
//output - level = 1, xp = 0, symbol = 'T'
Tower::Tower(int r, int c, int rng, int dmg)
    : row(r), col(c), range(rng), damage(dmg),
    level(1), xp(0), symbol('T')   // ASCII tower
{
}

int Tower::getRow() const { return row; }
int Tower::getCol() const { return col; }

int Tower::getRange() const { return range; }
int Tower::getDamage() const { return damage; }
int Tower::getLevel() const { return level; }

//returns symbol based on tower level
char Tower::getSymbol() const {
    switch (level) {
    case 1:  return 'T';
    case 2:  return 't';
    case 3:  return '#';
    case 4:  return '^';
    case 5:  return '@';
    case 6:  return '+';
    case 7:  return '%';
    case 8:  return '$';
    case 9:  return '&';
    case 10: return '?'; 
    default: return 'T';
    }
}


//returns true if enemy is within a tower's range
//input - enemy r,c
bool Tower::isInRange(int enemyRow, int enemyCol) const {
    return (std::abs(row - enemyRow) <= range &&
        std::abs(col - enemyCol) <= range);
}

// XP, every 3 kills = upgrade
void Tower::gainXP() {
    if (level >= MAX_LEVEL) return;

    xp++;

    int xpNeeded = 4 * level; //XP increase requirement

    if (xp >= xpNeeded) {
        upgrade();
    }
}


bool Tower::isReadyToUpgrade() const {
    return xp >= 3;
}

//damage and range increase for alternating level increases
void Tower::upgrade() {
    if (level >= MAX_LEVEL) return;

    level++;
    xp = 0;

    damage = 1 + (level / 2);
    range = 2 + (level / 3);
}




//splash attack
//input - primary target + vector of all enemies
//output - amount of enemies hit primary + splash
//primary = full damage, splash = 1 damage
int Tower::attackEnemy(Enemy* primary, std::vector<Enemy*>& enemies)
{
    if (!primary || primary->isDead()) return 0;

    //full hit on primary
    primary->damage(damage);

    int pr = primary->getRow();
    int pc = primary->getCol();

    int splashCount = 1; // includes primary

    //splash area
    for (Enemy* e : enemies) {
        if (!e || e->isDead()) continue;

        int r = e->getRow();
        int c = e->getCol();

        if (std::abs(r - pr) <= 1 && std::abs(c - pc) <= 1) {
            if (e != primary) {
                e->damage(1);
                splashCount++;
            }
        }
    }

    // XP only for killing primary
    if (primary->isDead()) {
        gainXP();
    }

    return splashCount;
}

//returns color code based on level
int Tower::getColor() const
{
    switch (level)
    {
    case 1:  return 14; 
    case 2:  return 7;  
    case 3:  return 11; 
    case 4:  return 10;
    case 5:  return 13; 
    case 6:  return 12; 
    case 7:  return 9;  
    case 8:  return 6; 
    case 9:  return 3;  
    case 10: return 15; 
    default: return 7; 
    }
}



