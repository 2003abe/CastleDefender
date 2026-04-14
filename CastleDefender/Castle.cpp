#include "Castle.h"

//constructor: castle position and starting health
Castle::Castle(int r, int c, int hp)
    : row(r), col(c), health(hp) {
}

//access casle state
int Castle::getRow() const { return row; }
int Castle::getCol() const { return col; }
int Castle::getHealth() const { return health; }

//reduces health by damage amount
void Castle::damage(int amount) {
    health -= amount;
}

//returns true when castle health <= 0
bool Castle::isDestroyed() const {
    return health <= 0;
}
