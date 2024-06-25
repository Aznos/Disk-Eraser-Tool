#ifndef GUI_H
#define GUI_H

#define FPS 60
#define FRAME_DELAY (1000 / FPS)
#define MAX_DISKS_PER_ROW 4

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "main.h"

void initGUI(int numDisks, struct DISK_INFO* disks);
void drawDiskRect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color);
void drawGrid(SDL_Renderer* renderer, int numDisks, int rectW, int rectH, SDL_Color color, SDL_Color textcolor, SDL_Texture* texture, TTF_Font* font, struct DISK_INFO* disks);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* file);
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);

#endif