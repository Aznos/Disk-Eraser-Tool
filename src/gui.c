#include "include/gui.h"
#include "include/util.h"
#include <stdio.h>

const int SCREENW = 1000;
const int SCREENH = 600;

void drawDiskRect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void drawGrid(SDL_Renderer* renderer, int numDisks, int rows, int cols, int rectW, int rectH, SDL_Color color) {
    int gridWidth = cols * rectW + (cols - 1) * (rectW / 4);
    int gridHeight = rows * rectH + (rows - 1) * (rectH / 4);

    int startX = (SCREENW - gridWidth) / 2;
    int startY = (SCREENH - gridHeight) / 2;

    for(int i = 0; i < numDisks; i++) {
        int row = i / cols;
        int col = i % cols;

        int x = startX + col * (rectW + rectW / 4);
        int y = startY + row * (rectH + rectH / 4);

        drawDiskRect(renderer, x, y, rectW, rectH, color);
    }
}

void initGUI() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Disk Erase Tool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENW, SCREENH, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 50, 54, 55, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Color rectColor = {255, 255, 255, 255};

    SDL_Event event;
    int running = 1;
    int rows = 2;
    int cols = (numDisks + rows - 1) / rows;
    int rectW = 150;
    int rectH = 150;
    while(running) {
        unsigned int frameStart = SDL_GetTicks();
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 50, 54, 55, 255);
        SDL_RenderClear(renderer);

        drawGrid(renderer, numDisks, rows, cols, rectW, rectH, rectColor);

        SDL_RenderPresent(renderer);

        unsigned int frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}