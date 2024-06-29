#include "../include/guidisk.h"
#include "../include/guiutil.h"
#include "../include/gui.h"
#include "../include/disk.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PROGRESS_BAR_WIDTH 300
#define PROGRESS_BAR_HEIGHT 30

void drawProgressBarGUI(SDL_Renderer* renderer, TTF_Font* font, unsigned long long totalWritten, unsigned long long totalSize, double elapsedTime) {
    double progress = (double)totalWritten / (double)totalSize;
    int barWidth = (int)(PROGRESS_BAR_WIDTH * progress);
    double writeSpeed = totalWritten / elapsedTime;
    double estimated = (totalSize - totalWritten) / writeSpeed;

    SDL_Color barColor = {255, 78, 78, 255};
    SDL_Color backgroundColor = {50, 54, 55, 255};
    SDL_Color textColor = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    SDL_Rect progressBarBackground = {50, 300, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &progressBarBackground);

    SDL_Rect progressBar = {50, 300, barWidth, PROGRESS_BAR_HEIGHT};
    SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
    SDL_RenderFillRect(renderer, &progressBar);

    char progressText[256];
    snprintf(progressText, sizeof(progressText), "%d%% - %.2f MB/s", (int)(progress * 100), writeSpeed / MB);

    SDL_Texture* progressTexture = renderText(renderer, font, progressText, textColor);
    if (progressTexture != NULL) {
        int textW, textH;
        SDL_QueryTexture(progressTexture, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = {50, 350, textW, textH};
        SDL_RenderCopy(renderer, progressTexture, NULL, &textRect);
        SDL_DestroyTexture(progressTexture);
    }

    int days = estimated / 86400;
    int hours = (estimated - (days * 86400)) / 3600;
    int minutes = (estimated - (days * 86400) - (hours * 3600)) / 60;
    int seconds = estimated - (days * 86400) - (hours * 3600) - (minutes * 60);

    if (days > 0) {
        snprintf(progressText, sizeof(progressText), "Estimated time remaining: %d days, %d hours, %d minutes, %d seconds", days, hours, minutes, seconds);
    } else if (hours > 0) {
        snprintf(progressText, sizeof(progressText), "Estimated time remaining: %d hours, %d minutes, %d seconds", hours, minutes, seconds);
    } else if (minutes > 0) {
        snprintf(progressText, sizeof(progressText), "Estimated time remaining: %d minutes, %d seconds", minutes, seconds);
    } else {
        snprintf(progressText, sizeof(progressText), "Estimated time remaining: %d seconds", seconds);
    }

    SDL_Texture* estimatedTimeTexture = renderText(renderer, font, progressText, textColor);
    if (estimatedTimeTexture != NULL) {
        int textW, textH;
        SDL_QueryTexture(estimatedTimeTexture, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = {50, 400, textW, textH};
        SDL_RenderCopy(renderer, estimatedTimeTexture, NULL, &textRect);
        SDL_DestroyTexture(estimatedTimeTexture);
    }

    SDL_RenderPresent(renderer);
}

void overwriteDiskGUI(const char* diskPath, bool random, unsigned long long size, SDL_Renderer* renderer, TTF_Font* font, int* running) {
    int disk = open(diskPath, O_WRONLY);
    if(disk < 0) {
        return;
    }

    unsigned char buffer[BUFFER_SIZE];
    unsigned long long totalWritten = 0;
    time_t startTime = time(NULL);
    time_t lastPrinttime = startTime;

    while(size > 0 && *running) {
        if(random) {
            for(unsigned int i = 0; i < sizeof(buffer); i++) {
                buffer[i] = "0123456789ABCDEF"[rand() % 16];
            }
        } else {
            memset(buffer, '0', sizeof(buffer));
        }

        ssize_t toWrite = size < BUFFER_SIZE ? size : BUFFER_SIZE;
        ssize_t written = write(disk, buffer, toWrite);
        if(written < 0) {
            break;
        }

        totalWritten += written;
        size -= written;

        time_t currentTime = time(NULL);
        if(difftime(currentTime, lastPrinttime) >= 1.0) {
            double elapsedTime = difftime(currentTime, startTime);
            drawProgressBarGUI(renderer, font, totalWritten, totalWritten + size, elapsedTime);
            lastPrinttime = currentTime;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = 0;
                close(disk);
                return;
            }
        }
    }

    close(disk);
}

void eraseDisk_GUI(struct DISK_INFO disk, int passes, SDL_Renderer* renderer, TTF_Font* font, int* running) {
    char diskPath[50];
    snprintf(diskPath, sizeof(diskPath), "/dev/%s", disk.path);

    for(int i = 0; i < passes && *running; i++) {
        if(i < 1) {
            overwriteDiskGUI(diskPath, false, disk.size, renderer, font, running);
        } else {
            overwriteDiskGUI(diskPath, true, disk.size, renderer, font, running);
        }

        if (*running) {
            SDL_Color textColor = {255, 78, 78, 255};
            char line[128];
            snprintf(line, sizeof(line), "Pass %d complete", i + 1);

            SDL_Texture* textTexture = renderText(renderer, font, line, textColor);
            if (textTexture != NULL) {
                int textW, textH;
                SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
                SDL_Rect textRect = {50, 450, textW, textH};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
        }
    }
}