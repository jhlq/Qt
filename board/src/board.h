#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "trianglegrid.h"

class Board
{
public:
    Board(int sideLength);
    TriangleGrid tg;
    int player;
    //Board deepcopy();
    bool isValidMove(Triangle &t);
    std::vector<Triangle> moves;
    std::vector<std::string> history;
    int greenCaptures;
    int blueCaptures;
    int greenTerritory;
    int blueTerritory;
};

#endif // BOARD_H
