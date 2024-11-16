#include "plane.h"

void *renderPlane(SDL_Renderer *renderer, Plane *plane)
{
    // Set the drawing color for the plane
    SDL_SetRenderDrawColor(renderer, plane->color.r, plane->color.g, plane->color.b, plane->color.a);

    SDL_Rect body = {plane->x, plane->y, plane->w, plane->h}; // Body is a vertical rectangle
    SDL_RenderDrawRect(renderer, &body);

    // SDL_Rect wings = {plane->x - (75 / 2), plane->y + 40, 100, 20}; // Wings
    // SDL_RenderFillRect(renderer, &wings);

    // SDL_Rect wing_part1 = {plane->x - (75 / 2) + 20, plane->y + 35, 20, 20}; // Wing part 1
    // SDL_RenderFillRect(renderer, &wing_part1);

    // SDL_Rect wing_part2 = {plane->x - (75 / 2) + 60, plane->y + 35, 20, 20}; // Wing part 2
    // SDL_RenderFillRect(renderer, &wing_part2);

    // SDL_Rect tail = {plane->x - (25 / 2), plane->y + 100, 50, 20}; // Tail
    // SDL_RenderFillRect(renderer, &tail);

    plane->hitbox = body;
    SDL_RenderCopy(renderer, plane->texture, NULL, &plane->hitbox);
}