#ifndef SDL_UTILS_H
#define SDL_UTILS_H
// Define the screen width and height
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "../Plane/plane.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void handleInput(Plane *plane);

#endif // SDL_UTILS_H