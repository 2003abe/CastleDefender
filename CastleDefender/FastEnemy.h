#pragma once
#ifndef FASTENEMY_H
#define FASTENEMY_H

#include "Enemy.h"

//quick, higher speed (moves 2 cells per turn), lower health
//F symbol
class FastEnemy : public Enemy {
public:
    //constructor, inputs (r,c), health from AI
    //output, health = baseHP-1, speed=2
    FastEnemy(int r, int c, int baseHP = 3);
    std::string getTypeName() const override { return "FastEnemy"; }
};


#endif


