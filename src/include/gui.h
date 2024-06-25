#ifndef GUI_H
#define GUI_H

#define FPS 60
#define FRAME_DELAY (1000 / FPS)
#define MAX_DISKS_PER_ROW 4

#include "SDL.h"

void initGUI();
void drawDiskRect(SDL_Renderer* renderer, int x, int , int w, int h, SDL_Color color);
void drawGrid(SDL_Renderer* renderer, int numDisks, int rectW, int rectH, SDL_Color color);

#endif