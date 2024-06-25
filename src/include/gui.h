#ifndef GUI_H
#define GUI_H

#define FPS 60
#define FRAME_DELAY (1000 / FPS)
#define MAX_DISKS_PER_ROW 4

#include "SDL.h"
#include "SDL_image.h"

void initGUI();
void drawDiskRect(SDL_Renderer* renderer, int x, int , int w, int h, SDL_Color color);
void drawGrid(SDL_Renderer* renderer, int numDisks, int rectW, int rectH, SDL_Color color, SDL_Texture* texture);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* file);

#endif