#ifndef BUILDING_H
#define BUILDING_H

#include <SDL2/SDL.h>
#include "../SDL_Utils/sdl_utils.h"
#include "../Interface/interface.h"

typedef struct
{
    int health;
    int x, y;
    int adults, elders, children;
    SDL_Texture *texture;
    SDL_Rect hitbox;
    int w, h;
} Building;

void spawnBuilding(Building *buildings, int *buildingCount, int maxBuildings, SDL_Renderer *renderer);
void renderBuildings(SDL_Renderer *renderer, Building *buildings, int buildingCount);
void updateBuildings(Building *buildings, int *buildingCount, int *score);

#endif