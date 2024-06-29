#include "../include/gui.h"
#include "../include/guiutil.h"
#include "../include/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int SCREENW = 1000;
const int SCREENH = 600;

void drawGrid(SDL_Renderer* renderer, int numDisks, int rectW, int rectH, SDL_Color color, SDL_Color textColor, SDL_Texture* texture, TTF_Font* font, struct DISK_INFO* disks) {
    if (numDisks > MAX_DISKS) {
        numDisks = MAX_DISKS;
    }

    int cols = (numDisks > MAX_DISKS_PER_ROW) ? MAX_DISKS_PER_ROW : numDisks;
    int rows = (numDisks + MAX_DISKS_PER_ROW - 1) / MAX_DISKS_PER_ROW;

    int gridWidth = cols * rectW + (cols - 1) * (rectW / 4);
    int gridHeight = rows * rectH + (rows - 1) * (rectH / 4);

    int startX = (SCREENW - gridWidth) / 2;
    int startY = (SCREENH - gridHeight) / 2;

    for (int i = 0; i < numDisks; i++) {
        int row = i / cols;
        int col = i % cols;

        int x = startX + col * (rectW + rectW / 4);
        int y = startY + row * (rectH + rectH / 4);

        drawDiskRect(renderer, x, y, rectW, rectH, color);

        char label[20];
        snprintf(label, sizeof(label), "Disk %d", i + 1);
        SDL_Texture* textTexture = renderText(renderer, font, label, textColor);
        if (textTexture != NULL) {
            int textW, textH;
            SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
            SDL_Rect textRect = {x + (rectW - textW) / 2, y - textH - 5, textW, textH};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
        }

        if (texture != NULL) {
            int textureW, textureH;
            SDL_QueryTexture(texture, NULL, NULL, &textureW, &textureH);
            SDL_Rect dstRect = {x + (rectW - textureW) / 2, y + (rectH - textureH) / 2, textureW, textureH};
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        }

        char sizeLabel[20];
        char buffer[35];
        snprintf(sizeLabel, sizeof(sizeLabel), "Size: %s", formatSize(buffer, disks[i].size));
        SDL_Texture* sizeTextTexture = renderText(renderer, font, sizeLabel, textColor);
        if (sizeTextTexture != NULL) {
            int textW, textH;
            SDL_QueryTexture(sizeTextTexture, NULL, NULL, &textW, &textH);
            SDL_Rect textRect = {x + (rectW - textW) / 2, y + rectH + 5, textW, textH};
            SDL_RenderCopy(renderer, sizeTextTexture, NULL, &textRect);
            SDL_DestroyTexture(sizeTextTexture);
        }
    }
}

void eraseDiskGUI(SDL_Renderer* renderer, struct DISK_INFO* disk, TTF_Font* font, int* running) {
    SDL_Color textColor = {255, 78, 78, 255};
    SDL_Color backgroundColor = {50, 54, 55, 255};
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    char line[128];
    snprintf(line, sizeof(line), "Erasing disk...");

    SDL_Texture* textTexture = renderText(renderer, font, line, textColor);
    if (textTexture != NULL) {
        int textW, textH;
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = {50, 50, textW, textH};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    snprintf(line, sizeof(line), "Disk erased successfully.");
    textTexture = renderText(renderer, font, line, textColor);
    if (textTexture != NULL) {
        int textW, textH;
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = {50, 150, textW, textH};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    *running = 0;
                    return;
            }
        }
    }
}

void drawDiskInfo(SDL_Renderer* renderer, struct DISK_INFO* disk, TTF_Font* font, int* running) {
    SDL_Color textColor = {255, 78, 78, 255};
    SDL_Color backgroundColor = {50, 54, 55, 255};
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    char line1[128];
    char line2[128];
    char buffer[35];
    formatSize(buffer, disk->size);
    snprintf(line1, sizeof(line1), "Are you sure you would like to delete the disk with size: %s?", buffer);
    snprintf(line2, sizeof(line2), "This action is irreversible and cannot be undone!");

    SDL_Texture* textTexture1 = renderText(renderer, font, line1, textColor);
    SDL_Texture* textTexture2 = renderText(renderer, font, line2, textColor);

    int textW1, textH1, textW2, textH2;
    SDL_QueryTexture(textTexture1, NULL, NULL, &textW1, &textH1);
    SDL_QueryTexture(textTexture2, NULL, NULL, &textW2, &textH2);

    int centerX = SCREENW / 2;
    int centerY = SCREENH / 2;

    SDL_Rect textRect1 = {centerX - textW1 / 2, centerY - textH1 - 20, textW1, textH1};
    SDL_Rect textRect2 = {centerX - textW2 / 2, centerY, textW2, textH2};

    SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
    SDL_RenderCopy(renderer, textTexture2, NULL, &textRect2);

    SDL_DestroyTexture(textTexture1);
    SDL_DestroyTexture(textTexture2);

    SDL_Color buttonColor = {255, 78, 78, 255};
    SDL_Color buttonTextColor = {255, 255, 255, 255};

    int buttonWidth = 150;
    int buttonHeight = 50;
    int buttonY = centerY + textH2 + 40;

    SDL_Texture* yesTexture = renderText(renderer, font, "Yes", buttonTextColor);
    SDL_Texture* noTexture = renderText(renderer, font, "No", buttonTextColor);

    SDL_Rect yesButtonRect = {centerX - buttonWidth - 20, buttonY, buttonWidth, buttonHeight};
    SDL_Rect noButtonRect = {centerX + 20, buttonY, buttonWidth, buttonHeight};

    drawDiskRect(renderer, yesButtonRect.x, yesButtonRect.y, yesButtonRect.w, yesButtonRect.h, buttonColor);
    drawDiskRect(renderer, noButtonRect.x, noButtonRect.y, noButtonRect.w, noButtonRect.h, buttonColor);

    int textW, textH;

    SDL_QueryTexture(yesTexture, NULL, NULL, &textW, &textH);
    SDL_Rect yesTextRect = {yesButtonRect.x + (yesButtonRect.w - textW) / 2, yesButtonRect.y + (yesButtonRect.h - textH) / 2, textW, textH};
    SDL_RenderCopy(renderer, yesTexture, NULL, &yesTextRect);

    SDL_QueryTexture(noTexture, NULL, NULL, &textW, &textH);
    SDL_Rect noTextRect = {noButtonRect.x + (noButtonRect.w - textW) / 2, noButtonRect.y + (noButtonRect.h - textH) / 2, textW, textH};
    SDL_RenderCopy(renderer, noTexture, NULL, &noTextRect);

    SDL_DestroyTexture(yesTexture);
    SDL_DestroyTexture(noTexture);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    *running = 0;
                    return;
                case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;

                    if (x >= yesButtonRect.x && x <= yesButtonRect.x + yesButtonRect.w && y >= yesButtonRect.y && y <= yesButtonRect.y + yesButtonRect.h) {
                        SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
                        SDL_RenderClear(renderer);
                        SDL_RenderPresent(renderer);
                        eraseDiskGUI(renderer, disk, font, running);
                        return;
                    } else if (x >= noButtonRect.x && x <= noButtonRect.x + noButtonRect.w && y >= noButtonRect.y && y <= noButtonRect.y + noButtonRect.h) {
                        return;
                    }
                }
                break;
            }
        }
    }
}

void initGUI(int numDisks, struct DISK_INFO* disks) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("Disk Erase Tool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENW, SCREENH, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 50, 54, 55, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Color rectColor = {38, 41, 41, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Texture* texture = loadTexture(renderer, "./src/assets/img/hdd (Nikita Golubev).png");
    TTF_Font* font = TTF_OpenFont("./src/assets/fonts/Roboto-Regular.ttf", 24);

    SDL_Event event;
    int running = 1;
    int rectW = 150;
    int rectH = 150;
    int showDetails = 0;
    struct DISK_INFO selectedDisk;
    while(running) {
        unsigned int frameStart = SDL_GetTicks();
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        int cols = (numDisks > MAX_DISKS_PER_ROW) ? MAX_DISKS_PER_ROW : numDisks;
                        int rows = (numDisks + MAX_DISKS_PER_ROW - 1) / MAX_DISKS_PER_ROW;
                        
                        int gridWidth = cols * rectW + (cols - 1) * (rectW / 4);
                        int gridHeight = rows * rectH + (rows - 1) * (rectH / 4);

                        int startX = (SCREENW - gridWidth) / 2;
                        int startY = (SCREENH - gridHeight) / 2;

                        for(int i = 0; i < numDisks; i++) {
                            int row = i / cols;
                            int col = i % cols;

                            int diskX = startX + col * (rectW + rectW / 4);
                            int diskY = startY + row * (rectH + rectH / 4);

                            if(x >= diskX && x <= diskX + rectW && y >= diskY && y <= diskY + rectH) {
                                selectedDisk = disks[i];
                                showDetails = 1;
                            }
                        }
                    }
                    break;
            }
        }

        if (showDetails) {
            drawDiskInfo(renderer, &selectedDisk, font, &running);
        } else {
            SDL_SetRenderDrawColor(renderer, 50, 54, 55, 255);
            SDL_RenderClear(renderer);

            drawGrid(renderer, numDisks, rectW, rectH, rectColor, textColor, texture, font, disks);

            SDL_RenderPresent(renderer);
        }

        unsigned int frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}