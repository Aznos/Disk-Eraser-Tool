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

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* file) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file);
    if(texture == NULL) {
        printf("Failed to load texture: %s\n", IMG_GetError());
    }

    return texture;
}

void drawGrid(SDL_Renderer* renderer, int numDisks, int rectW, int rectH, SDL_Color color, SDL_Texture* texture) {
    if(numDisks > MAX_DISKS) {
        numDisks = MAX_DISKS;
    }

    int cols = (numDisks > MAX_DISKS_PER_ROW) ? MAX_DISKS_PER_ROW : numDisks;
    int rows = (numDisks + MAX_DISKS_PER_ROW - 1) / MAX_DISKS_PER_ROW;
    
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

        if(texture != NULL) {
            int textureW, textureH;
            SDL_QueryTexture(texture, NULL, NULL, &textureW, &textureH);
            textureW *= 6;
            textureH *= 6;
            SDL_Rect dstRect = {x + (rectW - textureW) / 2, y + (rectH - textureH) / 2, textureW, textureH};
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        }
    }
}

void initGUI() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window* window = SDL_CreateWindow("Disk Erase Tool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENW, SCREENH, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 50, 54, 55, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Color rectColor = {255, 255, 255, 255};
    SDL_Texture* texture = loadTexture(renderer, "./src/assets/img/hdd.png");

    SDL_Event event;
    int running = 1;
    int rectW = 150;
    int rectH = 150;
    while(running) {
        unsigned int frameStart = SDL_GetTicks();
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 50, 54, 55, 255);
        SDL_RenderClear(renderer);

        drawGrid(renderer, numDisks, rectW, rectH, rectColor, texture);

        SDL_RenderPresent(renderer);

        unsigned int frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}