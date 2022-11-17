#include "display.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

SDL_Surface* screenSurface = NULL;
SDL_Window* window = NULL;

void clear_screen(void);
void start_screen(void);

void start_screen(void) {

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
}

void clear_screen(void) {
    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
    SDL_UpdateWindowSurface(window);
}
