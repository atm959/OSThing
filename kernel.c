#define true 1
#define false 0
#define bool char

bool enableMouseHandler = false;

unsigned int pointerX, pointerY;
bool mouseClicked = false;

#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 200

char tempVidBuffer[VIDEO_WIDTH * VIDEO_HEIGHT];

void copyTempBufferToGPUBuffer(){
    char* videoMemory = (char*)0xA0000;
    for(int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++){
        videoMemory[i] = tempVidBuffer[i];
    }
}

int winX, winY;
char selectedCol;

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
char pointerImageClicked[8 * 8] = {
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x09, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x09, 0x09, 0x09, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x09, 0x09, 0x09, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x09, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
};

char dataThingy[512 * 256];

void renderImage(int x, int y, int imageWidth, int imageHeight, char* image){
    for(int i = 0; i < imageWidth; i++){
        for(int j = 0; j < imageHeight; j++){
            char pixel = image[(j * imageWidth) + i];
            if(pixel != 0x00){
                tempVidBuffer[((y + j) * VIDEO_WIDTH) + (x + i)] = pixel - 1;
            }
        }
    }
}

/*
void renderButton(struct Button button){
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 16; j++){
            tempVidBuffer[((button.y + j) * VIDEO_WIDTH) + (button.x + i)] = 0x08; //Put the rectangle in video memory
        }
    }

    for(int i = 0; i < 33; i++){
        tempVidBuffer[(button.y * VIDEO_WIDTH) + (button.x + i)] = 0x00; //Top border
        tempVidBuffer[((button.y + 16) * VIDEO_WIDTH) + (button.x + i)] = 0x00; //Bottom border
        tempVidBuffer[((button.y + 17) * VIDEO_WIDTH) + (button.x + i + 1)] = 0x13; //Bottom shadow
    }
    for(int i = 0; i < 17; i++){
        tempVidBuffer[((button.y + i) * VIDEO_WIDTH) + button.x] = 0x00; //Left border
        tempVidBuffer[((button.y + i) * VIDEO_WIDTH) + (button.x + 32)] = 0x00; //Right border
        tempVidBuffer[((button.y + i + 1) * VIDEO_WIDTH) + (button.x + 33)] = 0x13; //Right shadow
    }
}
*/

inline void outb(short port, char val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

inline char inb(short port){
    char ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

inline short ins(short port){
    short ret;
    asm volatile ( "inw %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

inline void outs(short port, short val) {
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

struct IDT_Entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_Entry IDT[256];

void idt_init(){
    extern int load_idt();
    extern int divByZero();
    extern int irq0();
    extern int irq1();
    extern int irq2();
    extern int irq3();
    extern int irq4();
    extern int irq5();
    extern int irq6();
    extern int irq7();
    extern int irq8();
    extern int irq9();
    extern int irq10();
    extern int irq11();
    extern int irq12();
    extern int irq13();
    extern int irq14();
    extern int irq15();

    unsigned long divByZero_address;
    unsigned long irq0_address;
    unsigned long irq1_address;
    unsigned long irq2_address;
    unsigned long irq3_address;          
    unsigned long irq4_address; 
    unsigned long irq5_address;
    unsigned long irq6_address;
    unsigned long irq7_address;
    unsigned long irq8_address;
    unsigned long irq9_address;          
    unsigned long irq10_address;
    unsigned long irq11_address;
    unsigned long irq12_address;
    unsigned long irq13_address;
    unsigned long irq14_address;          
    unsigned long irq15_address;         
	unsigned long idt_address;
	unsigned long idt_ptr[2];

    /* remapping the PIC */
	outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 40);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    divByZero_address = (unsigned long)divByZero;
    IDT[0].offset_lowerbits = divByZero_address & 0xFFFF;
    IDT[0].selector = 0x08;
    IDT[0].zero = 0;
    IDT[0].type_attr = 0x8E;
    IDT[0].offset_higherbits = (divByZero_address & 0xFFFF0000) >> 16;

    irq0_address = (unsigned long)irq0; 
	IDT[32].offset_lowerbits = irq0_address & 0xffff;
	IDT[32].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[32].zero = 0;
	IDT[32].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[32].offset_higherbits = (irq0_address & 0xffff0000) >> 16;
 
	irq1_address = (unsigned long)irq1; 
	IDT[33].offset_lowerbits = irq1_address & 0xffff;
	IDT[33].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[33].zero = 0;
	IDT[33].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[33].offset_higherbits = (irq1_address & 0xffff0000) >> 16;
 
	irq2_address = (unsigned long)irq2; 
	IDT[34].offset_lowerbits = irq2_address & 0xffff;
	IDT[34].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[34].zero = 0;
	IDT[34].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[34].offset_higherbits = (irq2_address & 0xffff0000) >> 16;
 
	irq3_address = (unsigned long)irq3; 
	IDT[35].offset_lowerbits = irq3_address & 0xffff;
	IDT[35].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[35].zero = 0;
	IDT[35].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[35].offset_higherbits = (irq3_address & 0xffff0000) >> 16;
 
	irq4_address = (unsigned long)irq4; 
	IDT[36].offset_lowerbits = irq4_address & 0xffff;
	IDT[36].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[36].zero = 0;
	IDT[36].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[36].offset_higherbits = (irq4_address & 0xffff0000) >> 16;
 
	irq5_address = (unsigned long)irq5; 
	IDT[37].offset_lowerbits = irq5_address & 0xffff;
	IDT[37].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[37].zero = 0;
	IDT[37].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[37].offset_higherbits = (irq5_address & 0xffff0000) >> 16;
 
	irq6_address = (unsigned long)irq6; 
	IDT[38].offset_lowerbits = irq6_address & 0xffff;
	IDT[38].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[38].zero = 0;
	IDT[38].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[38].offset_higherbits = (irq6_address & 0xffff0000) >> 16;
 
	irq7_address = (unsigned long)irq7; 
	IDT[39].offset_lowerbits = irq7_address & 0xffff;
	IDT[39].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[39].zero = 0;
	IDT[39].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[39].offset_higherbits = (irq7_address & 0xffff0000) >> 16;
 
	irq8_address = (unsigned long)irq8; 
	IDT[40].offset_lowerbits = irq8_address & 0xffff;
	IDT[40].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[40].zero = 0;
	IDT[40].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[40].offset_higherbits = (irq8_address & 0xffff0000) >> 16;
 
	irq9_address = (unsigned long)irq9; 
	IDT[41].offset_lowerbits = irq9_address & 0xffff;
	IDT[41].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[41].zero = 0;
	IDT[41].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[41].offset_higherbits = (irq9_address & 0xffff0000) >> 16;
 
	irq10_address = (unsigned long)irq10; 
	IDT[42].offset_lowerbits = irq10_address & 0xffff;
	IDT[42].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[42].zero = 0;
	IDT[42].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[42].offset_higherbits = (irq10_address & 0xffff0000) >> 16;
 
	irq11_address = (unsigned long)irq11; 
	IDT[43].offset_lowerbits = irq11_address & 0xffff;
	IDT[43].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[43].zero = 0;
	IDT[43].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[43].offset_higherbits = (irq11_address & 0xffff0000) >> 16;
 
	irq12_address = (unsigned long)irq12; 
	IDT[44].offset_lowerbits = irq12_address & 0xffff;
	IDT[44].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[44].zero = 0;
	IDT[44].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[44].offset_higherbits = (irq12_address & 0xffff0000) >> 16;
 
	irq13_address = (unsigned long)irq13; 
	IDT[45].offset_lowerbits = irq13_address & 0xffff;
	IDT[45].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[45].zero = 0;
	IDT[45].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[45].offset_higherbits = (irq13_address & 0xffff0000) >> 16;
 
	irq14_address = (unsigned long)irq14; 
	IDT[46].offset_lowerbits = irq14_address & 0xffff;
	IDT[46].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[46].zero = 0;
	IDT[46].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[46].offset_higherbits = (irq14_address & 0xffff0000) >> 16;
 
    irq15_address = (unsigned long)irq15; 
	IDT[47].offset_lowerbits = irq15_address & 0xffff;
	IDT[47].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[47].zero = 0;
	IDT[47].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[47].offset_higherbits = (irq15_address & 0xffff0000) >> 16;
 
	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_Entry) * 256) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;
 
	load_idt(idt_ptr);
}

void divByZero_handler(void){
    outb(0x20, 0x20);

    for(int i = 0; i < (320 * 200); i++){
        tempVidBuffer[i] = 0x04;
    }

    copyTempBufferToGPUBuffer();

    while(true){}
}

void irq0_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq1_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq2_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq3_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq4_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq5_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq6_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq7_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq8_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI          
}

void irq9_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq10_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq11_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

unsigned char mouse_cycle=0;     //unsigned char
signed char mouse_byte[3];    //signed char
signed char mouse_x=0;         //signed char
signed char mouse_y=0;         //signed char

int mouse_time = 1000;

void irq12_handler(void) { //IRQ 12 - Mouse and Keyboard?
    if(enableMouseHandler == true){
        switch(mouse_cycle)
        {
            case 0:
                mouse_byte[0]=inb(0x60);
                mouse_cycle++;
                mouse_time = 1000;
                break;
            case 1:
                mouse_byte[1]=inb(0x60);
                mouse_cycle++;
                break;
            case 2:
                mouse_byte[2]=inb(0x60);
                mouse_x=mouse_byte[1];
                mouse_y=mouse_byte[2];

                pointerX += mouse_x;
                pointerY -= mouse_y;

                if(pointerX & 0xF0000000) pointerX = 0;
                if(pointerY & 0xF0000000) pointerY = 0;
                if(pointerX > (VIDEO_WIDTH - 8)) pointerX = (VIDEO_WIDTH - 8);
                if(pointerY > (VIDEO_HEIGHT - 8)) pointerY = (VIDEO_HEIGHT - 8);

                mouseClicked = mouse_byte[0] & 1;

                mouse_cycle=0;

            break;
        }
    }

    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq13_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq14_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq15_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

//Mouse code by SANiK
//License: Use as you wish, except to cause damage

inline void mouse_wait(unsigned char a_type) //unsigned char
{
  unsigned int _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

inline void mouse_write(unsigned char a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outb(0x60, a_write);
}

unsigned char mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inb(0x60);
}

void mouse_install() {
  unsigned char _status;  //unsigned char

  //Enable the auxiliary mouse device
  mouse_wait(1);
  outb(0x64, 0xA8);
 
  //Enable the interrupts
  mouse_wait(1);
  outb(0x64, 0x20);
  mouse_wait(0);
  _status=(inb(0x60) | 2);
  mouse_wait(1);
  outb(0x64, 0x60);
  mouse_wait(1);
  outb(0x60, _status);
 
  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge
 
  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge

  //Setup the mouse handler
  enableMouseHandler = true;
}

void renderPointerEditor(){
    int k = 0;

    //The "transparent checker" behind the large pointer
    for(int x = 0; x < 64; x++){
        for(int y = 0; y < 64; y++){
            tempVidBuffer[((y + winY + 1) * VIDEO_WIDTH) + (x + winX + 1)] = (((x / 2) + (y / 2)) % 2) ? 0x0F : 0x17;
        }
    }

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            char pixel = pointerImageClicked[(y * 8) + x];
            if(pixel != 0x00){
                for(int i = 0; i < 8; i++){
                    for(int j = 0; j < 8; j++){
                        tempVidBuffer[(((j + (y * 8)) + winY + 1) * VIDEO_WIDTH) + ((i + (x * 8)) + winX + 1)] = pixel - 1; //Draw the large mouse pointer
                    }
                }
            }
        }
    }

    //The pixel borders over the large pointer
    k = 0;
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 64; y++){
            tempVidBuffer[((y + winY + 1) * VIDEO_WIDTH) + ((x * 8) + winX + 1)] = (k % 2) ? 0x00 : 0x0F;
            k++;
        }
    }
    k = 0;
    for(int x = 0; x < 64; x++){
        for(int y = 0; y < 8; y++){
            tempVidBuffer[(((y * 8) + winY + 1) * VIDEO_WIDTH) + (x + winX + 1)] = (k % 2) ? 0x00 : 0x0F;
        }
        k++;
    }

    //The color palette
    k = 0;
    for(int y = 0; y < 16; y++){
        for(int x = 0; x < 16; x++){
            for(int i = 0; i < 8; i++){
                for(int j = 0; j < 8; j++){
                    tempVidBuffer[(((y * 8) + winY + 1 + j) * VIDEO_WIDTH) + ((x * 8) + winX + 64 + 2 + i)] = k - 1;
                }
            }
            k++;
        }
    }

    //The "transparent checker" on color 0 in the color palette
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            tempVidBuffer[((y + winY + 1) * VIDEO_WIDTH) + (x + winX + 64 + 2)] = (((x / 2) + (y / 2)) % 2) ? 0x0F : 0x17;
        }
    }

    //The "window border"
    for(int x = 0; x < (128 + 64 + 2); x++){
        tempVidBuffer[(winY * VIDEO_WIDTH) + (winX + x)] = 0x00;
        tempVidBuffer[((winY + 128 + 1) * VIDEO_WIDTH) + (winX + x)] = 0x00;
    }
    for(int y = 0; y < (128 + 2); y++){
        tempVidBuffer[((winY + y) * VIDEO_WIDTH) + winX] = 0x00;
        tempVidBuffer[((winY + y) * VIDEO_WIDTH) + (winX + 64 + 128 + 2)] = 0x00;
    }

    //The box around the selected color
    for(int x = 0; x < 8; x++){
        //tempVidBuffer[(((winY + 1) + ((selectedCol / 16) * 8)) * VIDEO_WIDTH) + 
    }
    for(int y = 0; y < 8; y++){
        
    }
}

bool detectDrive(){
    char val = inb(0x1F7);
    if(val & 0xFF) return false;
    return true;
}

int sectorCount;

bool initAndIdentifyDrive(){
    outb(0x1F6, 0xA0); //Select the master drive on the primary bus

    outb(0x1F2, 0); //Sectorcount
    outb(0x1F3, 0); //LBAlo
    outb(0x1F4, 0); //LBAmod
    outb(0x1F5, 0); //LBAhi

    outb(0x1F7, 0xEC); //IDENTIFY command

    char val = inb(0x1F7);
    if(val == 0) return false;

    while(inb(0x1F7) & 0x80){}

    if((inb(0x1F4) != 0) || (inb(0x1F5) != 0)) return false;

    while(!(val & 0x08) && !(val & 0x01)){
        val = inb(0x1F7);
    }

    if(val & 0x01) return false;

    for(int i = 0; i < 256; i++){
        dataThingy[i] = inb(0x1F0);
    }

    sectorCount = (dataThingy[60] << 16) | dataThingy[61];

    return true;
}

bool read256Sectors(int sector){
    outb(0x1F6, (0xE0 | (unsigned char)(sector >> 24)) | 0x40);
    
    outb(0x1F2, 0);
    outb(0x1F3, (unsigned char) sector);
    outb(0x1F4, (unsigned char) sector >> 8);
    outb(0x1F5, (unsigned char) sector >> 16);

    outb(0x1F7, 0x20); //READ SECTORS

    char val = inb(0x1F7);

    while((val & 0x80) || !(val & 0x08)){
        val = inb(0x1F7);
        if((val & 0x01) || (val & 0x20)) return false;
    }

    for(int i = 0; i < 256 * 256; i++){
        short val = ins(0x1F0);
        dataThingy[(512 * sector) + (i * 2)] = (val & 0xFF00) >> 8;
        dataThingy[(512 * sector) + ((i * 2) + 1)] = val & 0x00FF;
    }

    return true;
}

bool write256Sectors(int sector){
    outb(0x1F6, (0xE0 | (unsigned char)(sector >> 24)) | 0x40);
    
    outb(0x1F2, 0);
    outb(0x1F3, (unsigned char) sector);
    outb(0x1F4, (unsigned char) sector >> 8);
    outb(0x1F5, (unsigned char) sector >> 16);

    outb(0x1F7, 0x30); //WRITE SECTORS

    char val = inb(0x1F7);

    while((val & 0x80) || !(val & 0x08)){
        val = inb(0x1F7);
        if((val & 0x01) || (val & 0x20)) return false;
    }

    for(int i = 0; i < 256 * 256; i++){
        short valToWrite;
        valToWrite = (0xFF00 | 0xEE);
        outs(0x1F0, valToWrite);

        outb(0x1F7, 0xE7); //FLUSH CACHE
        while((val & 0x80) || !(val & 0x08)){
            val = inb(0x1F7);
            if((val & 0x01) || (val & 0x20)) return false;
        }

    }

    return true;
}

void kernelMain(){
    idt_init();

    mouse_install();

    enableMouseHandler = true;

    bool good = initAndIdentifyDrive();
    if(!good){
        for(int i = 0; i < 256; i++){
            dataThingy[i] = 0x04;
        }
    }
    good = read256Sectors(0);

    for(int i = 0; i < 512 * 256; i++){
        //dataThingy[i] = i % 256;
    }

    //good = write256Sectors(0);


    while(1){
        winX = 32;
        winY = 32;
        selectedCol++;

        for(int x = 0; x < VIDEO_WIDTH; x++){
            for(int y = 0; y < VIDEO_HEIGHT; y++){
                tempVidBuffer[(y * VIDEO_WIDTH) + x] = 0x0F; //Fill the background with white
            }
        }

        renderPointerEditor();

        renderImage(8, 8, 512 * 8, 1, dataThingy);

        int j = 0;
        int i = 3 / j; //Doesn't work for some reason, the OS continues to work

        if(mouseClicked){
            renderImage(pointerX, pointerY, 8, 8, pointerImageClicked);
        } else {
            renderImage(pointerX, pointerY, 8, 8, pointerImage);
        }

        copyTempBufferToGPUBuffer();
    }
}