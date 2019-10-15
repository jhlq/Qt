#include "triangle.h"

Triangle::Triangle(int x,int y)
{
    this->x=x;
    this->y=y;
    this->player=0;
    this->markedDead=false;
}

bool Triangle::alive(){
    return player>0 && !markedDead;
}
/*
bool Triangle::sameAs(Triangle *t){
    if (t->player==this->player){
        return true;
    } else if (t->player>0&&this->player>0){
        return false;
    } else if ((this->markedDead||this->player==0)&&(t->markedDead||t->player==0)){
        return true;
    }
    return false;
}*/
bool Triangle::sameAs(Triangle *t){
    return this->player==t->player || ((this->markedDead||this->player==0)&&(t->markedDead||t->player==0));
}
