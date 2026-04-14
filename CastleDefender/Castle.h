#pragma once
#ifndef CASTLE_H
#define CASTLE_H

//player's castle on the grid
//stores position and health
class Castle {
private:
    int row, col;
    int health;


public:
    //constructor, takes (r,c) of castle, starting health
    Castle(int r, int c, int hp = 100);

    //output castle position, current HP
    int getRow() const;
    int getCol() const;
    int getHealth() const;

    //damage
    //input, amount of health to subtract from castle
    void damage(int amount);

    //output = true if HP <= 0
    bool isDestroyed() const;
};

#endif
