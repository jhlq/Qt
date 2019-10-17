#ifndef TRIANGLEGRID_H
#define TRIANGLEGRID_H

#include <vector>
#include <string>
#include "triangle.h"

class TriangleGrid
{
private:
    std::vector<std::vector<Triangle>> triangles; //change this to 1D array for performance?

public:
    TriangleGrid(int sideLength);
    Triangle get(int x,int y);
    void set(int x,int y,int player);
    bool has(int x,int y);
    int sideLength;
    void setUpGrid();
    int nTriangles();
    std::vector<Triangle> adjacent(Triangle &triangle);
    std::vector<Triangle> adjacent(std::vector<Triangle> &group);
    std::vector<Triangle> adjacentPieces(Triangle &tri);
    std::vector<Triangle> adjacentPieces(std::vector<Triangle> &group);
    std::vector<Triangle> getConnected(Triangle &tri);
    std::vector<Triangle> getGroup(Triangle &tri);
    std::vector<Triangle> getCluster(std::vector<Triangle> &group);
    std::vector<Triangle> getCluster(Triangle &tri);
    int liberties(std::vector<Triangle> &group);
    int liberties(Triangle &tri);
    void removeGroup(std::vector<Triangle> &group,Triangle &capturer);
    std::string historyString();
};

#endif // TRIANGLEGRID_H
