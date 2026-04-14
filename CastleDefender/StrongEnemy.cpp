#include "StrongEnemy.h"

//constructor, higher health, speed = 1, symbol=S
StrongEnemy::StrongEnemy(int r, int c, int baseHP)
    : Enemy(r, c, baseHP + 4, 1, 'S')  
{
}

