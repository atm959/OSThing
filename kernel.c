int pointerX, pointerY;

char pointerImage[8 * 8] = {
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x10, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x10, 0x10, 0x10, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x10, 0x10, 0x10, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x10, 0x10, 0x10, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x10, 0x10, 0x10, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

void renderPointer(){
    char* videoMemory = (char*)0xA0000;

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            char pixel = pointerImage[(y * 8) + x];
            if(pixel != 0x00){
                videoMemory[((pointerY + y) * 320) + (pointerX + x)] = pixel - 1;
            }
        }
    }
}

void renderButton(int x, int y){
    char* videoMemory = (char*)0xA0000;

    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 16; j++){
            videoMemory[((y + j) * 320) + (x + i)] = 0x08;
        }
    }

    for(int i = 0; i < 33; i++){
        videoMemory[(y * 320) + (x + i)] = 0x00;
        videoMemory[((y + 16) * 320) + (x + i)] = 0x00;
        videoMemory[((y + 17) * 320) + (x + i + 1)] = 0x12;
    }
    for(int i = 0; i < 17; i++){
        videoMemory[((y + i) * 320) + x] = 0x00;
        videoMemory[((y + i) * 320) + (x + 32)] = 0x00;
        videoMemory[((y + i + 1) * 320) + (x + 33)] = 0x12;
    }
}

void kernelMain(){
    char* videoMemory = (char*)0xA0000;

    for(int x = 0; x < 320; x++){
        for(int y = 0; y < 200; y++){
            videoMemory[(y * 320) + x] = 0x0F;
        }
    }
    
    for(int x = 0; x < 5; x++){
        for(int y = 0; y < 5; y++){
            renderButton(x * 33, y * 17);
        }
    }
    renderPointer();

    while(1){
    }
}