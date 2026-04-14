#pragma once
#ifndef STRONGENEMY_H
#define STRONGENEMY_H

#include "Enemy.h"
#include <string>

//strong enemy, higher health, normal speed
//health=baseHP+4, S symbol
class StrongEnemy : public Enemy {
public:
    //constructor, input (r,c), baseHP from AI
    //output, slow, high health enemy
    StrongEnemy(int r, int c, int baseHP = 3);


    std::string getTypeName() const override { return "StrongEnemy"; }
};

#endif
