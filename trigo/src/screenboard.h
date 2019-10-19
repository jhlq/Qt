#ifndef SCREENBOARD_H
#define SCREENBOARD_H

#include <vector>
#include "screentriangle.h"
#include "board.h"
#include <QObject>

class ScreenBoard : public QObject
{
    Q_OBJECT
public:
    ScreenBoard(int sideLength,int _unitSize,int _offsetX,int _offsetY);
    Board board;
    std::vector<std::vector<ScreenTriangle>> triangles;
    int unitSize;
    int offsetX;
    int offsetY;

    ScreenTriangle makeTriangle(int x,int y);
    void setUpGrid();
signals:
    //void placedmove();
    void modifiedmoves();
    void modifiedscore();
private slots:
    void clickevent(int pixX,int pixY);
    void undo();
    void pass();
    void score();
};

#endif // SCREENBOARD_H
