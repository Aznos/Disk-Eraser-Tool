#include "../include/guiutil.h"

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

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if(surface == NULL) {
        printf("Failed to render text: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if(texture == NULL) {
        printf("Failed to create texture from surface: %s\n", SDL_GetError());
    }

    return texture;
}

char* formatSize(char* buffer, size_t size) {
    if(size >= TB) {
        snprintf(buffer, 35, "%.2f TB", (double)size / TB);
    } else if(size >= GB) {
        snprintf(buffer, 35, "%.2f GB", (double)size / GB);
    } else if(size >= MB) {
        snprintf(buffer, 35, "%.2f MB", (double)size / MB);
    } else {
        snprintf(buffer, 35, "%.2f KB", (double)size / KB);
    }

    return buffer;
}