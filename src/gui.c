#include "include/gui.h"
#include <stdio.h>

const int SCREENW = 1000;
const int SCREENH = 600;

void drawDiskRect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void initGUI() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Disk Erase Tool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENW, SCREENH, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 50, 54, 55, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Color rectColor = {255, 0, 0, 255};

    SDL_Event event;
    int running = 1;
    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
        }

        drawDiskRect(renderer, 200, 150, 600, 300, rectColor);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}