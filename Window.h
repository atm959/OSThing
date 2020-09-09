#ifndef WINDOW_H
#define WINDOW_H

#include "Point.h"
#include "WindowElement.h"

#include "WindowCloseWidget.h"

class Window {
public:
    Window(){}
    Window(char* title, Point pos, Point size){
        this->title = title;
        this->pos = pos;
        this->size = size;
    }
    void renderBorder(){
        if(this->size.x < 96) this->size.x = 105;
        if(this->size.y < 32) this->size.y = 32;

        char* vidMem = (char*)0xA0000;

        for(unsigned short x = this->pos.x; x < (this->pos.x + this->size.x); x++){
            for(unsigned short y = this->pos.y; y < (this->pos.y + 9); y++){
                vidMem[(y * 320) + x] = 8;
            }
        }

        for(unsigned short x = this->pos.x; x < (this->pos.x + this->size.x); x++){
            for(unsigned short y = this->pos.y + 9; y < (this->pos.y + this->size.y + 9); y++){
                vidMem[(y * 320) + x] = 13;
            }
        }

        for(unsigned short x = this->pos.x; x < (this->pos.x + this->size.x); x++){
            vidMem[(this->pos.y * 320) + x] = 0;
            vidMem[((this->pos.y + 9) * 320) + x] = 0;
            vidMem[((this->pos.y + this->size.y + 9) * 320) + x] = 0;
        }

        for(unsigned short y = this->pos.y; y < (this->pos.y + this->size.y + 9) + 1; y++){
            vidMem[(y * 320) + this->pos.x] = 0;
            vidMem[(y * 320) + (this->pos.x + this->size.x)] = 0;
        }
    }
    void renderCloseWidget(){
        for(unsigned short x = 0; x < 9; x++){
            for(unsigned short y = 0; y < 10; y++){
                unsigned short xPos = ((this->pos.x + this->size.x) - 8) + x;
                unsigned short yPos = this->pos.y + y;
                unsigned char pixel = windowCloseWidget[(y * 9) + x];
                vidMemMode13[(yPos * 320) + xPos] = pixel;
            }
        }
    }
    void render(){
        this->renderBorder();
        this->renderCloseWidget();
        renderText(this->title, 16, this->pos.x + 1, this->pos.y + 1, 15);

        /*
        for(unsigned short i = 0; i < 256; i++){
            if(winElements[i] != nullptr){
                winElements[i]->render();
            } else {
                break;
            }
        }
        */
    }

    Point pos;
    Point size;
    char* title;
};

#endif