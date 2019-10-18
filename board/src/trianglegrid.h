#ifndef TRIANGLEGRID_H
#define TRIANGLEGRID_H

#include <vector>
#include <string>
#include "triangle.h"

class TriangleGrid
{
private:
    std::vector<std::vector<Triangle>> triangles; //change this to 1D array for performance?

friend class Board;

public:
    TriangleGrid(int sideLength);
    Triangle get(int x,int y);
    void set(int x,int y,int player);
    bool has(int x,int y);
    //void addCaptured(int x,int y,Triangle &captured);
    int sideLength;
    void setUpGrid();
    int nTriangles();
    std::vector<Triangle> adjacent(const Triangle &triangle);
    std::vector<Triangle> adjacent(const std::vector<Triangle> &group);
    std::vector<Triangle> adjacentPieces(const Triangle &tri);
    std::vector<Triangle> adjacentPieces(const std::vector<Triangle> &group);
    std::vector<Triangle> getConnected(const Triangle &tri);
    std::vector<Triangle> getGroup(const Triangle &tri);
    std::vector<Triangle> getCluster(const std::vector<Triangle> &group);
    std::vector<Triangle> getCluster(const Triangle &tri);
    int liberties(const std::vector<Triangle> &group);
    int liberties(const Triangle &tri);
    void removeGroup(std::vector<Triangle> &group,Triangle &capturer);
    std::string historyString();
};

#endif // TRIANGLEGRID_H
