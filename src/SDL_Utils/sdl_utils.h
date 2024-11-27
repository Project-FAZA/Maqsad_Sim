#ifndef SDL_UTILS_H
#define SDL_UTILS_H
// Define the screen width and height
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "../Plane/plane.h"
#include "../Bullet/bullet.h"
#include "../Music/music.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void handleInput(Plane *plane, Bullet *bullets, int *bulletCount);
int getButtonClick(Uint8 scancode);
void getUsername(char username[4], SDL_Renderer *renderer, int *charCount);
void getMenuOpt(SDL_Renderer *renderer, int *selected);
int getCreditOpt();

#endif // SDL_UTILS_H