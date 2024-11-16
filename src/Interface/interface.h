#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "../SDL_Utils/sdl_utils.h"

typedef struct
{
    int x, y;
    SDL_Texture *texture[20]; // 20 Frames max
    int currFrame;
    Uint32 lastFrameTime;
} Explosion;

SDL_Texture *loadImage(SDL_Renderer *renderer, const char *imagePath, int *w, int *h);
void loadExplosion(Explosion *explosionArray, int *explosionArrayCount, SDL_Renderer *renderer, const char *folderPath, int totalFrames, int x, int y);
void updateExplosion(Explosion *explosionArray, int *explosionArrayCount, int totalFrames);
void renderExplosion(SDL_Renderer *renderer, Explosion *explosionArray, int *explosionArrayCount);
void transitionBackground(SDL_Renderer *renderer, SDL_Texture *background1, SDL_Texture *background2, int alpha);
void renderBg(SDL_Renderer *renderer, SDL_Texture *bg, SDL_Texture *bg_new, int *transitioning, Uint32 *startTime, int *alpha);

#endif