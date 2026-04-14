#ifndef BLOCK_H
#define BLOCK_H

//single cells on the game's grid
//holds a symbol for rendering
class Block {
private:
    char symbol; //for enemy types, towers

public:
    //initialises empty block
    //output = '.'
    Block();

    //getter - returns current character
    char getSymbol() const;

    //setter - input is a symbol for display
    void setSymbol(char s);
};

#endif
