#include "trianglegrid.h"

TriangleGrid::TriangleGrid(int sideLength)
{
    this->sideLength=sideLength;
    this->setUpGrid();
}
void TriangleGrid::setUpGrid(){
    for (int yt = 0; yt < sideLength; yt++) {
        std::vector<Triangle> v;
        for (int xt = 0; xt <= 2*sideLength-2*yt-2; xt++) {
            v.push_back(Triangle(xt,yt));
        }
        this->triangles.push_back(v);
    }
}
int TriangleGrid::nTriangles(){
    int n=0;
    for(std::vector<Triangle> yv : this->triangles) {
        for(Triangle xt : yv) {
            n++;
        }
    }
    return n;
}
