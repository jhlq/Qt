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
    bool isValidMove(int x,int y,int player);
    bool isValidMove(Triangle &t);
    void placeMove(int x,int y);
    void placeMove(int x,int y,int player);
    std::vector<Triangle> moves;
    std::vector<std::string> history;
    int greenCaptures;
    int blueCaptures;
    int greenTerritory;
    int blueTerritory;
};

#endif // BOARD_H
