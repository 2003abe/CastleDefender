#include <windows.h>
#include "Grid.h"
#include <iostream>
#include <tuple>

//allows coloured output
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//constructor, creates empty grid
Grid::Grid(int r, int c)
    : rows(r), cols(c), splashColorPtr(nullptr)
{
    board.resize(rows, std::vector<Block>(cols));
}

//registers pointer to splash-colour list in Game
void Grid::setSplashColorSource(std::vector<std::tuple<int, int, int>>* ptr)
{
    splashColorPtr = ptr;
}

//draws grid row by row
//outputs coloured board
void Grid::print() const {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {

            char cell = board[r][c].getSymbol();
            int color = COLOR_GRAY;

            //base object colours
            switch (cell)
            {
            case 'E': color = COLOR_GREEN; break;
            case 'F': color = COLOR_RED; break;
            case 'S': color = COLOR_MAGENTA; break;
            case 'C': color = COLOR_CYAN; break;
            default:  color = COLOR_GRAY; break;
            }

            //splash override
            if (splashColorPtr != nullptr)
            {
                for (auto& tpl : *splashColorPtr)
                {
                    int sr = std::get<0>(tpl);
                    int sc = std::get<1>(tpl);
                    int scolor = std::get<2>(tpl);

                    if (sr == r && sc == c)
                    {
                        color = scolor;
                        break;
                    }
                }
            }

            SetConsoleTextAttribute(hConsole, color);
            std::cout << cell << " ";
            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
        }

        std::cout << "\n";
    }
}

//for end game summaries
void Grid::displayFinal() const
{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            std::cout << board[r][c].getSymbol();
        }
        std::cout << "\n";
    }
}

//writes symbol into grid
//input (r,c)
void Grid::setCell(int r, int c, char symbol) {
    board[r][c].setSymbol(symbol);
}

//gets symbol stored at (r,c)
char Grid::getCell(int r, int c) const {
    return board[r][c].getSymbol();
}

int Grid::getRows() const { return rows; }
int Grid::getCols() const { return cols; }
