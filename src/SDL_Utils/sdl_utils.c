#include "sdl_utils.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        printf("Error SDL2_image Initialization");
        exit(1);
    }

    // Create window
    *window = SDL_CreateWindow("SDL2 Plane", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Create renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void handleInput(Plane *plane)
{
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_UP])
        plane->y -= 5;
    if (keystates[SDL_SCANCODE_DOWN])
        plane->y += 5;
    if (keystates[SDL_SCANCODE_LEFT])
        plane->x -= 5;
    if (keystates[SDL_SCANCODE_RIGHT])
        plane->x += 5;
}