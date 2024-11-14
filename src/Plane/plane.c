#include "plane.h"

SDL_Texture *load_image(SDL_Renderer *renderer, const char *imagePath)
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
    SDL_FreeSurface(surface); // Free the surface after creating the texture

    if (texture == NULL)
    {
        printf("Error creating texture: %s\n", SDL_GetError());
    }

    return texture;
}

void *renderPlane(SDL_Renderer *renderer, Plane *plane)
{
    // Set the drawing color for the plane
    SDL_SetRenderDrawColor(renderer, plane->color.r, plane->color.g, plane->color.b, plane->color.a);

    // SDL_Rect body = {plane->x, plane->y, 25, 100}; // Body is a vertical rectangle
    // SDL_RenderFillRect(renderer, &body);

    // SDL_Rect wings = {plane->x - (75 / 2), plane->y + 40, 100, 20}; // Wings
    // SDL_RenderFillRect(renderer, &wings);

    // SDL_Rect wing_part1 = {plane->x - (75 / 2) + 20, plane->y + 35, 20, 20}; // Wing part 1
    // SDL_RenderFillRect(renderer, &wing_part1);

    // SDL_Rect wing_part2 = {plane->x - (75 / 2) + 60, plane->y + 35, 20, 20}; // Wing part 2
    // SDL_RenderFillRect(renderer, &wing_part2);

    // SDL_Rect tail = {plane->x - (25 / 2), plane->y + 100, 50, 20}; // Tail
    // SDL_RenderFillRect(renderer, &tail);

    SDL_Rect plane_rect = {plane->x, plane->y, 50, 50};
    SDL_RenderCopy(renderer, plane->texture, NULL, &plane_rect);
}