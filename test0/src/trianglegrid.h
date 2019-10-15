#ifndef TRIANGLEGRID_H
#define TRIANGLEGRID_H

#include <vector>
#include "triangle.h"

class TriangleGrid
{
public:
    TriangleGrid(int sideLength);
    std::vector<std::vector<Triangle>> triangles;
    std::vector<Triangle> clicked;
    int sideLength;
    void setUpGrid();
    int nTriangles();
};

#endif // TRIANGLEGRID_H
