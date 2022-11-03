
#include <stdbool.h>
#include <stdio.h>
#include "display.h"
#include "font.h"

void load_font_data(__uint8_t* ram);
void fetch(__uint16_t* PC, __uint16_t* opcode, __uint8_t* ram);
void decode(__uint16_t* PC, __uint16_t* opcode, __uint8_t* ram);

int main(int argc, char* args[]) {
    // VARIABLE DECLARATION
    __uint8_t RAM[4096] = {0};
    // program counter
    __uint16_t PC = 0x200;
    //index register
    int I = 0;
    int stack[4096] = {0};
    __uint8_t delay_timer = 255;
    __uint8_t sound_timer = 255;
    __uint16_t opcode = 0x0;

    // variable registers
    __uint8_t V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF;

    start_screen();
    load_font_data(RAM);
    fetch(&PC, &opcode, RAM);
    decode(&PC, &opcode, RAM);
    return 0;
}


void load_font_data(__uint8_t* ram) {
    for(int i = 0x50; i <= 0x9F; ++i) 
        ram[i] = font_data[i - 0x50];
}

void fetch(__uint16_t* PC, __uint16_t* opcode,  __uint8_t* ram) {
    *opcode = ram[*PC++];
    *opcode <<= 8;
    *opcode |= ram[*PC++];
}

void decode(__uint16_t* PC, __uint16_t* opcode,  __uint8_t* ram) {
    //TODO big switch case
}