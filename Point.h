#ifndef POINT_H
#define POINT_H

class Point {
public:
    Point(){
        this->x = 0;
        this->y = 0;
    }
    Point(unsigned short x, unsigned short y){
        this->x = x;
        this->y = y;
    }

    unsigned short x, y;
};

#endif