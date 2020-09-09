#ifndef COMMON_H
#define COMMON_H

#include "Font.h"

char* vidMemText = (char*)0xB8000;
char* vidMemMode13 = (char*)0xA0000;

void renderText(char* text, unsigned char length, unsigned short x, unsigned short y, unsigned char color){
    for(unsigned char i = 0; i < length; i++){
        char c = text[i] - 33;
        unsigned short xPosInImage1 = c * 5;
        for(unsigned short xP = 0; xP < 5; xP++){
            for(unsigned short yP = 0; yP < 8; yP++){
                unsigned char pixel = font[(yP * 320) + (xPosInImage1 + xP)];
                if(pixel == 1){
                    pixel = color;
                    unsigned short screenX = (xP + (i * 6)) + x;
                    unsigned short screenY = yP + y;
                    vidMemMode13[(screenY * 320) + screenX] = pixel;
                }
            }
        }
    }
}

#endif