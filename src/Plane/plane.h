#ifndef PLANE_H
#define PLANE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef struct
{
    int x, y;        // Position of the plane
    SDL_Color color; // Color of the plane
    SDL_Texture *texture;
} Plane;

SDL_Texture *load_image(SDL_Renderer *renderer, const char *imagePath);
void *renderPlane(SDL_Renderer *renderer, Plane *plane);
void handleInput(Plane *plane);

#endif // PLANE_H