#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "../SDL_Utils/sdl_utils.h"
#include "../Game/gameState.h"

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
void renderBg(SDL_Renderer *renderer, SDL_Texture *bg);
void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color);
void menuMode(SDL_Renderer *renderer, GameState *gameState);
void nameMode(SDL_Renderer *renderer, GameState *gameState, char username[4], int *charCount);
void playMode(SDL_Renderer *renderer, int score, char username[4]);

#endif