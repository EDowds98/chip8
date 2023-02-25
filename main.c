
#include <SDL2/SDL.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "font.h"

void load_font_data(__uint8_t* ram);
void load_program(__uint8_t* ram, char* filename);
void fetch(__uint16_t* PC, __uint16_t* opcode, __uint8_t* ram);
void decode(__uint16_t* PC, __uint16_t opcode, __uint8_t* ram, __uint8_t* registers, int index_reg);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

SDL_Surface* screenSurface = NULL;
SDL_Window* window = NULL;

    // VARIABLE DECLARATION
    __uint8_t RAM[4096] = {0};
    // program counter
    __uint16_t PC = 0x200;
    // index register
    int I = 0;
    int stack[4096] = {0};
    __uint8_t delay_timer = 255;
    __uint8_t sound_timer = 255;
    __uint16_t opcode = 0x0000;

    // variable registers
    __uint8_t V[0x10] = {0};

int main(int argc, char* argv[]) {

    load_font_data(RAM);
    load_program(RAM, argv[1]);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("SDL could not initialise! SDL_Error: %s\n", SDL_GetError());
    else {
        // create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
            printf("Windows could not be created! SDL_Error: %s\n", SDL_GetError());
        else {
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(window);
        }
    }

    while (1) {
        fetch(&PC, &opcode, RAM);
        sleep(1);
        decode(&PC, opcode, RAM, V, I);
    }
    SDL_Event e;
    bool quit = false;
    while (quit == false) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }
    }
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
    return 0;
}
// TODO rethink how the SDL works here, maybe needs totally restructured

void load_font_data(__uint8_t* ram) {
    for (int i = 0x50; i <= 0x9F; ++i)
        ram[i] = font_data[i - 0x50];
}

void load_program(__uint8_t* ram, char* filename) {
    FILE* program_data;
    if ((program_data = fopen(filename, "rb")) == NULL) {
        printf("ch8 file not loaded\n");
        printf("Error %d \n", errno);
    }
    fread((ram + 0x200), 1, sizeof(ram), program_data);
    fclose(program_data);
}

void fetch(__uint16_t* PC, __uint16_t* opcode, __uint8_t* ram) {
    *opcode = ram[*PC];
    printf("first half of opcode: %X PC: %X\n", *opcode, *PC);
    (*PC)++;
    *opcode <<= 8;
    *opcode |= ram[*PC];
    printf("second half of opcode: %X PC: %X\n", *opcode, *PC);
    (*PC)++;
}

void decode(__uint16_t opcode, __uint8_t* ram, __uint8_t* registers, int index_reg) {
    // split into 4 nibbles
    printf("here is opcode: %X %d\n", opcode, opcode);

    // i am big endian here
    __uint8_t nibble1, nibble2, nibble3, nibble4;
    nibble1 = ((opcode >> 12) & 0xF);
    nibble2 = ((opcode >> 8) & 0xF);
    nibble3 = ((opcode >> 4) & 0xF);
    nibble4 = (opcode & 0xF);

    printf("print bytes individually; should get %x: %x %x %x %x\n", opcode, nibble1, nibble2, nibble3, nibble4);
    // TODO big switch case
    __uint8_t X = ((opcode >> 8) & 0x000F);
    __uint8_t Y = ((opcode >> 4) & 0x000F);
    __uint8_t N = opcode & 0x000F;
    __uint8_t NN = opcode & 0x00FF;
    __uint16_t NNN = opcode & 0x0FFF;

    int x_coord = (V[X] * 10) % 640;
    int y_coord = (V[Y] * 10) % 320;

    switch (opcode & 0xF000) {
        case 0x0:   // 0x00E0: clear screen
            printf("clear screen instruction\n");
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
            SDL_UpdateWindowSurface(window);
            break;
        case 0x1000:  // 1NNN: JUMP
            *PC = NNN;
            printf("program counter after JUMP: %x\n", *PC);
            break;
        case 0x6000:  // 6XNN: set register VX
            printf("In set register instruction 0x6XNN\n");
            registers[X] = NN;
        case 0x7000:    // 7XNN: Add
            printf("in ADD instruction 0x7XNN\n");
            registers[X] += NN;
            break;
        case 0xA000: // ANNN: Set index register I
            printf("in Set index register instruction 0xA000\n");
            index_reg = NNN;
            break;
        case 0xD000: // DXYN: Display/Draw
            break;
    }
}

// command to check hex file: xxd -l 120 -ps -c 20 'IBM Logo.ch8'
