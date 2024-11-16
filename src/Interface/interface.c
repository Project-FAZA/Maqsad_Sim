#include "interface.h"

SDL_Texture *loadImage(SDL_Renderer *renderer, const char *imagePath, int *w, int *h)
{
    // Load the image as an SDL_Surface
    SDL_Surface *surface = IMG_Load(imagePath);
    if (surface == NULL)
    {
        printf("Error loading image: %s\n", IMG_GetError());
        return NULL; // Return NULL if loading fails
    }

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    (*w) = surface->w;
    (*h) = surface->h;

    SDL_FreeSurface(surface); // Free the surface after creating the texture

    if (texture == NULL)
    {
        printf("Error creating texture: %s\n", SDL_GetError());
    }

    return texture;
}