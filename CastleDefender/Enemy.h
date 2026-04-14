#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <string>

//base class
//represents enemies moving on the grid
//stores position, health, speed, symbol, unique ID

class Enemy {
protected:
    int row, col;
    int health;
    int speed;
    char symbol;

    //ID for each enemy
    static int NEXT_ID;
    int id;

public:
    //constructor
    //inputs, starting row/col, health, speed, symbol
    //output, enemy with unique ID
    Enemy(int r, int c, int hp = 3, int spd = 1, char sym = 'E');

    int getID() const { return id; }  //for tracking

    //polymorphic, returns class name
    virtual std::string getTypeName() const { return "Enemy"; }

    int getRow() const;
    int getCol() const;

    void setRow(int r);
    void setCol(int c);

    int getSpeed() const;

    void damage(int amount);    // subtracts HP
    bool isDead() const;        // true if HP <= 0

    virtual char getSymbol() const;  //symbol for grid
};

#endif


