#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture *loadImage(SDL_Renderer *renderer, const char *imagePath, int *w, int *h);

#endif