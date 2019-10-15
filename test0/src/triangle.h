#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

class Triangle
{
public:
    Triangle(int x,int y,int pixX,int pixY);

    int x;
    int y;
    int pixX;
    int pixY;
    int player;
    int prevPlayer;
    std::vector<Triangle> captured;
    bool markedDead;

    bool alive();
    bool sameAs(Triangle *t);
};

#endif // TRIANGLE_H
