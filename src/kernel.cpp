extern "C" {

char* vidMemText = (char*)0xB8000;
const char hexTable[17] = "0123456789ABCDEF";

void clearScreen(){
    char* vidMem = (char*)0xB8000;
    for(unsigned int i = 0; i < (80*25)*2; i++){
        vidMemText[i] = 0x00;
    }
}

void print32Bit(unsigned int value, unsigned char x, unsigned char y){
    unsigned char shiftVal = 28;
    unsigned int andVal = 0xF0000000;
    unsigned int tempVal = 0;
    unsigned char tempX = x + 2;

    vidMemText[((y * 80) + x) * 2] = '0';
    vidMemText[(((y * 80) + x) * 2) + 1] = 0x0F;
    vidMemText[((y * 80) + (x + 1)) * 2] = 'x';
    vidMemText[(((y * 80) + (x + 1)) * 2) + 1] = 0x0F;

    for(int counter = 0; counter < 8; counter++){
        tempVal = value & andVal;
        tempVal = tempVal >> shiftVal;

        vidMemText[((y * 80) + tempX) * 2] = hexTable[tempVal];
        vidMemText[(((y * 80) + tempX) * 2) + 1] = 0x0F;

        shiftVal -= 4;
        andVal = andVal >> 4;

        tempX++;
    }
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

void kernelMain(){
    clearScreen();

    placeText((char*)"PLACE TEXT NOW!", 0, 0, 0x0F);
    print32Bit(0xDEADBEEF, 0, 1);
}

}