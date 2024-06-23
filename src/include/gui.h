#ifndef GUI_H
#define GUI_H

#define FPS 60
#define FRAME_DELAY (1000 / FPS)

#include "SDL2/SDL.h"

void initGUI();
void drawDiskRect(SDL_Renderer* renderer, int x, int , int w, int h, SDL_Color color);
void drawGrid(SDL_Renderer* renderer, int numDisks, int rows, int cols, int rectW, int rectH, SDL_Color color);

#endif