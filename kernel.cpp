extern "C" {

#include "Common.h"

#include "Port.h"

#include "Font.h"
#include "WindowCloseWidget.h"

#include "Window.h"

void fillBG(){
    char* vidMem = (char*)0xA0000;
    for(unsigned short x = 0; x < 320; x++){
        for(unsigned short y = 0; y < 200; y++){
            vidMem[(y * 320) + x] = 15;
        }
    }
}

const char hexTable[17] = "0123456789ABCDEF";
const char binTable[3] = "01";

char getCMOSSeconds(){
    outb(0x70, 0x00);
    return inb(0x71);
}

char getCMOSMinutes(){
    outb(0x70, 0x02);
    return inb(0x71);
}

char getCMOSHours(){
    outb(0x70, 0x04);
    return inb(0x71);
}

#include "interruptExceptionHandlers.cpp"

void clearScreen(){
    char* vidMem = (char*)0xB8000;
    for(unsigned int i = 0; i < (80*25)*2; i++){
        vidMem[i] = 0x00;
    }
}

void print8Bit(unsigned char value, unsigned char x, unsigned char y){
    unsigned char shiftVal = 4;
    unsigned char andVal = 0xF0;
    unsigned char tempVal = 0;
    unsigned char tempX = x;

    for(int counter = 0; counter < 2; counter++){
        tempVal = value & andVal;
        tempVal = tempVal >> shiftVal;

        vidMemText[((y * 80) + tempX) * 2] = hexTable[tempVal];
        vidMemText[(((y * 80) + tempX) * 2) + 1] = 0x0F;

        shiftVal -= 4;
        andVal = andVal >> 4;

        tempX++;
    }

    vidMemText[((y * 80) + tempX) * 2] = 'H';
    vidMemText[(((y * 80) + tempX) * 2) + 1] = 0x0F;
}

void print16Bit(unsigned short value, unsigned char x, unsigned char y){
    unsigned char shiftVal = 12;
    unsigned short andVal = 0xF000;
    unsigned short tempVal = 0;
    unsigned char tempX = x;

    for(int counter = 0; counter < 4; counter++){
        tempVal = value & andVal;
        tempVal = tempVal >> shiftVal;

        vidMemText[((y * 80) + tempX) * 2] = hexTable[tempVal];
        vidMemText[(((y * 80) + tempX) * 2) + 1] = 0x0F;

        shiftVal -= 4;
        andVal = andVal >> 4;

        tempX++;
    }

    vidMemText[((y * 80) + tempX) * 2] = 'H';
    vidMemText[(((y * 80) + tempX) * 2) + 1] = 0x0F;
}

void print32Bit(unsigned int value, unsigned char x, unsigned char y){
    unsigned char shiftVal = 28;
    unsigned int andVal = 0xF0000000;
    unsigned int tempVal = 0;
    unsigned char tempX = x;

    for(int counter = 0; counter < 8; counter++){
        tempVal = value & andVal;
        tempVal = tempVal >> shiftVal;

        vidMemText[((y * 80) + tempX) * 2] = hexTable[tempVal];
        vidMemText[(((y * 80) + tempX) * 2) + 1] = 0x0F;

        shiftVal -= 4;
        andVal = andVal >> 4;

        tempX++;
    }

    vidMemText[((y * 80) + tempX) * 2] = 'H';
    vidMemText[(((y * 80) + tempX) * 2) + 1] = 0x0F;
}

void printBinary8Bit(unsigned char value, unsigned char x, unsigned char y){
    unsigned char shiftVal = 7;
    unsigned char andVal = 0b10000000;
    unsigned char tempVal = 0;
    unsigned char tempX = x;

    for(int counter = 0; counter < 8; counter++){
        tempVal = value & andVal;
        tempVal = tempVal >> andVal;

        vidMemText[((y * 80) + tempX) * 2] = binTable[tempVal];
        vidMemText[(((y * 80) + tempX) * 2) + 1] = 0x0F;

        shiftVal -= 1;
        andVal = andVal >> 1;

        tempX++;
    }

    vidMemText[((y * 80) + tempX) * 2] = 'B';
    vidMemText[(((y * 80) + tempX) * 2) + 1] = 0x0F;
}

void placeText(char* text, unsigned char x, unsigned char y, unsigned char attribute){
    unsigned char tempX = x;
    unsigned int tempPointer = 0;
    while(text[tempPointer] != '\0'){
        vidMemText[((y * 80) + tempX) * 2] = text[tempPointer];
        vidMemText[(((y * 80) + tempX) * 2) + 1] = attribute;

        tempX++;
        tempPointer++;
    }
}

void shiftScreenUp(){
	for(int i = 0; i < (((80*24)*2)-1); i++){
		vidMemText[i] = vidMemText[i + 160];
	}
	for(int i = 0; i < 160; i++){
		vidMemText[i + ((80*24)*2)] = 0x00;
	}
}

extern void halt();

typedef struct IMAGE {
    unsigned short x, y;
    unsigned short w, h;
    unsigned char* data;
} IMAGE;

typedef struct TEXTSTRING {
    unsigned short x, y;
    char text[32];
    unsigned char color;
} TEXTSTRING;

/*
void drawImage(WINDOW* win, IMAGE* img){
    char* vidMem = (char*)0xA0000;

    for(unsigned short x = 0; x < img->w; x++){
        for(unsigned short y = 0; y < img->h; y++){
            unsigned short xPos = win->x + img->x + x + 1;
            unsigned short yPos = win->y + img->y + y + 8;
            if((xPos > (win->x)) && (xPos < (win->x + win->w))){
                if((yPos > (win->y + 8)) && (yPos < (win->y + win->h + 8))){
                    unsigned char pixel = img->data[(y * img->w) + x];
                    vidMem[(yPos * 320) + xPos] = pixel;
                }
            }
        }
    }
}

void renderTextInWindow(WINDOW* win, TEXTSTRING* textString){
    char* vidMem = (char*)0xA0000;

    for(unsigned char i = 0; i < 32; i++){
        char c = textString->text[i] - 33;
        unsigned short xPosInImage1 = c * 5;
        for(unsigned short xP = 0; xP < 5; xP++){
            for(unsigned short yP = 0; yP < 8; yP++){
                unsigned char pixel = font[(yP * 320) + (xPosInImage1 + xP)];
                if(pixel == 1){
                    pixel = textString->color;
                    unsigned short screenX = (xP + (i * 6)) + textString->x + win->x;
                    unsigned short screenY = yP + textString->y + win->y + 8;
                    if((screenX > (win->x)) && (screenX < (win->x + win->w))){
                        if((screenY > (win->y + 8)) && (screenY < (win->y + win->h + 8))){
                            vidMem[(screenY * 320) + screenX] = pixel;
                        }
                    }
                }
            }
        }
    }
}

void drawWindow(WINDOW* win){
    renderText(win->title, 16, win->x + 1, win->y + 1, (unsigned char)win->x);
}
*/

Window* windows[128];

#define PIT_CHANNEL0_DATA 0x40
#define PIT_COMMAND 0x43

void initTimerInterrupt(){
    unsigned short divisor = 1193180 / 100;
    unsigned char l = (unsigned char)(divisor & 0xFF);
    unsigned char h = (unsigned char)((divisor >> 8) & 0xFF);

    outb(PIT_COMMAND, 0x36);

    outb(PIT_CHANNEL0_DATA, l);
    outb(PIT_CHANNEL0_DATA, h);
}

Window testWindow;

void kernelMain(){
    clearScreen();

    idt_init();

    initTimerInterrupt();

    fillBG();

    renderText((char*)"MADE BY ATM959 IN 2020", 22, 189, 192, 0);

    testWindow = Window((char*)"TEST WINDOW C++", Point(8, 8), Point(128, 128));
    testWindow.render();

    while(1){

    }

    //halt();
}

}