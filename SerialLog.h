#ifndef SERIALLOG_H
#define SERIALLOG_H

#include "Port.h"

#define COM1_BASE 0x3F8

void serialInit(){
    outb(COM1_BASE + 1, 0x00);    // Disable all interrupts
    outb(COM1_BASE + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1_BASE + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1_BASE + 1, 0x00);    //                  (hi byte)
    outb(COM1_BASE + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1_BASE + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1_BASE + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() {
   return inb(COM1_BASE + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   outb(COM1_BASE, a);
}

void serialLog(char* text){
    int i = 0;
    while(text[i] != '\0'){
        write_serial(text[i]);
        i++;
    }
}

#endif