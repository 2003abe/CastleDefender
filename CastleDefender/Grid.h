#ifndef GRID_H
#define GRID_H

#include <windows.h> 
#include <vector>
#include "Block.h"
#include <tuple>

//for colour rendering
extern HANDLE hConsole;


//colour codes for drawing objects
enum Color {
    COLOR_DEFAULT = 7,
    COLOR_GRAY = 8,
    COLOR_GREEN = 10,
    COLOR_YELLOW = 14,
    COLOR_RED = 12,
    COLOR_MAGENTA = 13,
    COLOR_CYAN = 11
};

//represents the battle field
//stores block objects (matrix), draws them to console
//splash colour for splash damage effects
class Grid {
private:
    int rows;
    int cols;

    //2D grid, stores symbols
    std::vector<std::vector<Block>> board;

    //visualising splash hits (row, col, colour)
    std::vector<std::tuple<int, int, int>>* splashColorPtr;


public:
    //constructor, takes (r,c), outputs empty grid
    Grid(int r = 20, int c = 20);

    //reference to splash colours list
    void setSplashColorSource(std::vector<std::tuple<int, int, int>>* ptr);

    //draws grid at current cursor
    void print() const;

    //raw gri, no colour or cursor repositioning
    void displayFinal() const;

    //setter for a cell's symbol
    void setCell(int r, int c, char symbol);

    //reads symbol
    char getCell(int r, int c) const;

    int getRows() const;
    int getCols() const;
};

#endif

