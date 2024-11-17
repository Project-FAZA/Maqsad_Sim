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
    SDL_Rect hitbox;
    int w, h; // Width and Height
} Plane;

void *renderPlane(SDL_Renderer *renderer, Plane *plane);

#endif // PLANE_H