
#include <SDL2/SDL.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "font.h"

void load_font_data(__uint8_t* ram);
void load_program(__uint8_t* ram, char* filename);
void fetch(__uint16_t* PC, __uint16_t* opcode, __uint8_t* ram);
void decode(__uint16_t* PC, __uint16_t opcode, __uint8_t* ram);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

SDL_Surface* screenSurface = NULL;
SDL_Window* window = NULL;

int main(int argc, char* argv[]) {
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
    __uint8_t V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF;

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
        decode(&PC, opcode, RAM);
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

void decode(__uint16_t* PC, __uint16_t opcode, __uint8_t* ram) {
    // Get individual nibbles
    printf("here is opcode: %X %d\n", opcode, opcode);
    __uint16_t nibble1, nibble2, nibble3, nibble4;
    nibble1 = opcode >> 12;
    nibble2 = opcode >> 8;
    nibble3 = opcode >> 4;
    nibble4 = opcode << 12;
    printf("print all nibbles individually; should get 00e0: %x %x %x %x\n", nibble1, nibble2, nibble3, nibble4);
    // TODO big switch case
    switch (opcode) {
        case 0x00e0:
            printf("here1\n");
            screenSurface = SDL_GetWindowSurface(window);
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
            SDL_UpdateWindowSurface(window);
            break;
        case 0xd01f:
            break;
    }
}