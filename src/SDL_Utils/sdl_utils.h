#ifndef SDL_UTILS_H
#define SDL_UTILS_H
// Define the screen width and height
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#include <SDL2/SDL.h>
#include <stdio.h>
#include "../Plane/plane.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void handleInput(Plane *plane);

#endif // SDL_UTILS_H