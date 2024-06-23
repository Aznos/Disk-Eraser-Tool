#ifndef GUI_H
#define GUI_H

#include "SDL2/SDL.h"

void initGUI();
void drawDiskRect(SDL_Renderer* renderer, int x, int , int w, int h, SDL_Color color);

#endif