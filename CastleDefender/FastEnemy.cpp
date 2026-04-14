#include "FastEnemy.h"

//constructor, lower health, faster, F symbol
FastEnemy::FastEnemy(int r, int c, int baseHP)
    : Enemy(r, c, baseHP - 1, 2, 'F') 
{
}

