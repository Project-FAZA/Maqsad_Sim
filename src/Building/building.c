#include "building.h"

void spawnBuilding(Building *buildings, int *buildingCount, int maxBuildings, SDL_Renderer *renderer)
{
    // Load the texture once and reuse it
    static SDL_Texture *texture = NULL;
    if (texture == NULL)
    {
        texture = loadImage(renderer, "images/buildings/1.png", &buildings[*buildingCount].w, &buildings[*buildingCount].h);
    }
    if ((*buildingCount) < maxBuildings)
    {
        int x = rand() % (SCREEN_WIDTH - 100); // Random x between 0 and SCREEN_WIDTH - building width (100)

        buildings[*buildingCount].texture = texture;

        if (!buildings[*buildingCount].texture)
        {
            printf("Failed to load building texture\n");
            return; // Skip this spawn if texture loading fails
        }

        buildings[*buildingCount].x = x;
        buildings[*buildingCount].y = -100; // Spawn just above the screen (off-screen)

        buildings[*buildingCount].health = 30; // Lower health for moving buildings
        (*buildingCount)++;
    }
}

void renderBuildings(SDL_Renderer *renderer, Building *buildings, int buildingCount)
{
    for (int i = 0; i < buildingCount; i++)
    {
        if (buildings[i].health <= 0)
        {
            continue; // Skip destroyed buildings
        }

        SDL_Rect body = {buildings[i].x, buildings[i].y, 100, 100}; // Larger size for buildings
        buildings[i].hitbox = body;

        SDL_RenderCopy(renderer, buildings[i].texture, NULL, &(buildings[i].hitbox));
    }
}

void updateBuildings(Building *buildings, int *buildingCount, int *score)
{
    int speed = 2 + ((*score) / 100); // Increase speed slightly as score increases

    for (int i = 0; i < *buildingCount; i++)
    {
        buildings[i].y += speed; // Move the building downward

        // Remove buildings that go off-screen or are destroyed
        if (buildings[i].y > SCREEN_HEIGHT || buildings[i].health <= 0)
        {
            // Shift the array to remove the building
            for (int j = i; j < (*buildingCount) - 1; j++)
            {
                buildings[j] = buildings[j + 1];
            }
            (*buildingCount)--;
            i--; // Adjust index to recheck shifted building
            *(score)-=20;
        }
    }
}