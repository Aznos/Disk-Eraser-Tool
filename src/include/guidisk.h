#ifndef GUIDISK_H
#define GUIDISK_H

#include <stdbool.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "main.h"

void drawProgressBarGUI(SDL_Renderer* renderer, TTF_Font* font, unsigned long long totalWritten, unsigned long long totalSize, double elapsedTime);
void overwriteDiskGUI(const char* diskPath, bool random, unsigned long long size, SDL_Renderer* renderer, TTF_Font* font, int* running);
void eraseDisk_GUI(struct DISK_INFO disk, int passes, SDL_Renderer* renderer, TTF_Font* font, int* running);

#endif