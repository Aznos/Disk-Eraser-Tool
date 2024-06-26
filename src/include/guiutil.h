#ifndef GUIUTIL_H
#define GUIUTIL_H

#include "gui.h"

void drawDiskRect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* file);
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);
char* formatSize(char* buffer, size_t size);

#endif