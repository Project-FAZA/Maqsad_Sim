#ifndef PLANE_H
#define PLANE_H

#include <SDL2/SDL.h>

typedef struct
{
    int x, y;        // Position of the plane
    SDL_Color color; // Color of the plane
} Plane;

void renderPlane(SDL_Renderer *renderer, Plane *plane);
void handleInput(Plane *plane);

#endif // PLANE_H