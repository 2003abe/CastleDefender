#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include "Game.h"

//program entry point
//creates instance of Game, and runs full gameplay sequence
int main() { 

    Game game;              //game control
    game.setup();           //initialises board and systems
    game.placeTowers();     //player chooses tower locations
    game.start();           //runs entire game loop

    return 0;
}
