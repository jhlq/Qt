#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

class Triangle
{
public:
    Triangle(int x,int y);
    Triangle(int x,int y,int player);
    int x;
    int y;
    int player;
    //int prevPlayer;
    //std::vector<Triangle> captured;
    bool markedDead;

    bool alive();
    bool sameTenantAs(const Triangle &t);
    bool operator==(const Triangle& t);
};

#endif // TRIANGLE_H
