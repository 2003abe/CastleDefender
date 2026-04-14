#include "Enemy.h"

int Enemy::NEXT_ID = 1;

//constructor, assigns position, stats, unique ID
Enemy::Enemy(int r, int c, int hp, int spd, char sym)
    : row(r), col(c), health(hp), speed(spd), symbol(sym)
{
    id = NEXT_ID++; 
}

int Enemy::getRow() const { return row; }
int Enemy::getCol() const { return col; }

void Enemy::setRow(int r) { row = r; }
void Enemy::setCol(int c) { col = c; }

int Enemy::getSpeed() const { return speed; }

//reduce health by damage amount
void Enemy::damage(int amount) { health -= amount; }

//eliminated (health <= 0)
bool Enemy::isDead() const { return health <= 0; }

//base symbol
char Enemy::getSymbol() const {
    return symbol;
}
