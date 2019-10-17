#include "board.h"

Board::Board(int sideLength) : tg(sideLength)
{
    tg=TriangleGrid(sideLength);
    int player=1;
    //std::vector<std::tring> history; //add status for blank board?
    int greenCaptures=0;
    int blueCaptures=0;
    int greenTerritory=0;
    int blueTerritory=0;
}
bool Board::isValidMove(int x,int y,int player){
    Triangle t=Triangle(x,y,player);
    return isValidMove(t);
}
bool Board::isValidMove(Triangle &t){ //refactor?
    if (!tg.has(t.x,t.y)){
        return false;
    }
    Board bc=Board(*this);
    bc.tg.set(t.x,t.y,t.player);
    Triangle tri=bc.tg.get(t.x,t.y);
    std::vector<Triangle> adj=bc.tg.adjacent(tri);
    for (int a=0;a<adj.size();a++){
            if (adj[a].alive()){
            std::vector<Triangle> g=bc.tg.getGroup(adj[a]);
            if (bc.tg.liberties(g)==0){
                bc.tg.removeGroup(g,tri);
            }
        }
    }
    std::vector<Triangle> group=tg.getGroup(tri);
    if (bc.tg.liberties(group)==0){
        return false;
    }
    return true;
}
void placeMove(int x,int y){
    placeMove(x,y,player);
}
void placeMove(int x,int y,int player){
    if (!isValidMove(x,y,player)){
        return;
    }
}
