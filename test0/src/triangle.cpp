#include "triangle.h"

Triangle::Triangle(int x,int y,int pixX,int pixY)
{
    this->x=x;
    this->y=y;
    this->pixX=pixX;
    this->pixY=pixY;
    this->player=0;
}

bool Triangle::alive(){
    return player>0 && !markedDead;
}
bool Triangle::sameAs(Triangle *t){
    if (t->player==this->player){
        return true;
    } else if (t->player>0&&this->player>0){
        return false;
    } else if ((this->markedDead||this->player==0)&&(t->markedDead||t->player==0)){
        return true;
    }
    return false;
}
