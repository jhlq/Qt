#include "board.h"
#include "util.h"

Board::Board(int sideLength) : tg(sideLength)
{
    tg=TriangleGrid(sideLength);
    player=1;
    //std::vector<std::tring> history; //add status for blank board?
    stones[0]=0;
    stones[1]=0;
    captures[0]=0;
    captures[1]=0;
    territory[0]=0;
    territory[1]=0;
}
void Board::reset(){
    tg=TriangleGrid(tg.sideLength);
    player=1;
    history.clear();
    moves.clear();
    stones[0]=0;
    stones[1]=0;
    captures[0]=0;
    captures[1]=0;
    territory[0]=0;
    territory[1]=0;
}
void Board::removeCapturedBy(const Triangle tri){
    //Triangle tri=tg.get(x,y);
    std::vector<Triangle> adj=tg.adjacent(tri);
    for (int a=0;a<adj.size();a++){
        int ap=adj[a].player;
        if (adj[a].alive()&&ap!=tri.player){
            std::vector<Triangle> g=tg.getGroup(adj[a]);
            if (tg.liberties(g)==0){
                tg.removeGroup(g);
                stones[ap-1]-=g.size();
                captures[tri.player-1]+=g.size();
            }
        }
    }
}

bool Board::isValidMove(int x,int y,int player){
    Triangle t=Triangle(x,y,player);
    return isValidMove(t);
}
bool Board::isValidMove(const Triangle &t){ //refactor?
    if (!tg.has(t.x,t.y) || tg.get(t.x,t.y).player!=0){
        return false;
    }
    Board bc=Board(*this);
    bc.tg.set(t.x,t.y,t.player);
    Triangle tri=bc.tg.get(t.x,t.y);
    //bc.removeCapturedBy(tri);
    std::vector<Triangle> adj=bc.tg.adjacent(tri);
    for (int a=0;a<adj.size();a++){
            if (adj[a].alive()&&adj[a].player!=tri.player){
            std::vector<Triangle> g=bc.tg.getGroup(adj[a]);
            if (bc.tg.liberties(g)==0){
                bc.tg.removeGroup(g);
            }
        }
    }
    std::vector<Triangle> group=bc.tg.getGroup(tri);
    if (bc.tg.liberties(group)==0){
        return false;
    }
    std::string h=bc.tg.historyString();
    if (contains(this->history,h)){
        return false;
    }
    return true;
}
int Board::otherPlayer(){
    if (player==1){
        return 2;
    } else {
        return 1;
    }
}
int Board::otherPlayer(int p){
    if (p==1){
        return 2;
    } else {
        return 1;
    }
}
void Board::switchPlayer(){
    player=otherPlayer();
}
bool Board::placeMove(int x,int y){
    bool b=placeMove(x,y,player);
    if (b) {
        switchPlayer();
    }
    return b;
}
bool Board::placeMove(int x,int y,int p){
    if (x<0){ //pass
        moves.push_back(Triangle(x,y,p));
        return true;
    }
    if (!isValidMove(x,y,p)){
        return false;
    }
    tg.set(x,y,p);
    Triangle tri=tg.get(x,y);
    removeCapturedBy(tri);
    /*std::vector<Triangle> adj=tg.adjacent(tri);
    for (int a=0;a<adj.size();a++){
            if (adj[a].alive()&&adj[a].player!=tri.player){
            std::vector<Triangle> g=tg.getGroup(adj[a]); //put this in a function?
            if (tg.liberties(g)==0){
                captures[p-1]+=g.size();
                tg.removeGroup(g,tri);
            }
        }
    }*/
    history.push_back(tg.historyString());
    moves.push_back(tri);
    stones[p-1]+=1;
    return true;
}
std::string Board::state(){
    std::string s=std::to_string(tg.sideLength)+";";
    for (Triangle move : moves){
        s+=std::to_string(move.x)+","+std::to_string(move.y)+","+std::to_string(move.player)+";";
    }
    return s;
}
void Board::placeMoves(){
    std::vector<Triangle> m=moves;
    int p=player;
    reset();
    for (Triangle move : m){
        placeMove(move.x,move.y,move.player);
    }
    player=p;
}
void Board::undo(){
    if (!history.empty()){
        history.pop_back();
        moves.pop_back();
        switchPlayer();
        placeMoves();
    }
}
void Board::pass(){
    moves.push_back(Triangle(-1,-1,player));
    switchPlayer();
}

void Board::score(){
    std::vector<Triangle> checked;
    int scores[2]={0,0};
    for (int y=0;y<tg.triangles.size();y++){
        for (int x=0;x<tg.triangles[y].size();x++){
            Triangle tri=tg.triangles[y][x];
            if ((tri.player==0||tri.markedDead) && !contains(checked,tri)){
                std::vector<Triangle> c=tg.getConnected(tri);
                for (int ci=0;ci<c.size();ci++){
                    checked.push_back(c[ci]);
                }
                std::vector<Triangle> adj=tg.adjacent(c);
                int p=adj[0].player;
                if (!adj.empty() && p>0){
                    bool oneplayer=true;
                    for (int adji=0;adji<adj.size();adji++){
                        if (adj[adji].player!=p){
                            oneplayer=false;
                            break;
                        }
                    }
                    if (oneplayer){
                        scores[p-1]+=c.size();
                    }

                }
            }

        }
    }
    territory[0]=scores[0];
    territory[1]=scores[1];
}
void Board::markDeadStones(int x,int y){
    markDeadStones(tg.get(x,y));
}
void Board::markDeadStones(const Triangle &tri){
    std::vector<Triangle> c=tg.getCluster(tri);
    bool dead=!tri.markedDead;
    int a=-1;
    if (dead){
        a=1;
    }
    for (int i=0;i<c.size();i++){
        Triangle t=c[i];
        if (t.player==tri.player){
            tg.triangles[t.y][t.x].markedDead=dead;
            stones[t.player-1]-=a;
            captures[otherPlayer(t.player)-1]+=a;
        }
    }
}
