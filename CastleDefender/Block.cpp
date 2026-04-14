#include "Block.h"

//default empty block
Block::Block() : symbol('.') {}


//getter - returns stored symbol
char Block::getSymbol() const {
    return symbol;
}

//setter - new character for block
void Block::setSymbol(char s) {
    symbol = s;
}

