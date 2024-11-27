#include "building.h"

void spawnBuilding(Building *buildings, int *buildingCount, int maxBuildings, SDL_Renderer *renderer)
{
    // Load the textures once and reuse them
    static SDL_Texture *textures[3] = {NULL, NULL, NULL};
    const char *textureFiles[3] = {
        "images/buildings/1.png",
        "images/buildings/2.png",
        "images/buildings/3.png"};

    // Load textures if they are not already loaded
    for (int i = 0; i < 3; i++)
    {
        if (textures[i] == NULL)
        {
            textures[i] = loadImage(renderer, textureFiles[i], NULL, NULL);
            if (textures[i] == NULL)
            {
                printf("Failed to load building texture %s\n", textureFiles[i]);
                return;
            }
        }
    }

    if ((*buildingCount) < maxBuildings)
    {
        // Randomly select a texture
        int i = (rand() % 9);

        if (i < 5) // 50% chance
            i = 2;
        else if (i < 8) // 30% chance (next 3 values: 5, 6, 7)
            i = 0;
        else // 20% chance (last 2 values: 8, 9)
            i = 1;

        int x = rand() % (SCREEN_WIDTH - 60); // Random x between 0 and SCREEN_WIDTH - building width (50-60)

        buildings[*buildingCount].texture = textures[i];

        if (i == 0) // School
        {
            buildings[*buildingCount].adults = 5;
            buildings[*buildingCount].children = 10;
        }
        else if (i == 1) // Orphanage
        {
            buildings[*buildingCount].adults = 1;
            buildings[*buildingCount].children = 15;
        }
        else if (i == 2) // Hospital
        {
            buildings[*buildingCount].adults = 2;
            buildings[*buildingCount].children = 2;
            buildings[*buildingCount].elders = 4;
        }

        if (!buildings[*buildingCount].texture)
        {
            printf("Failed to assign building texture\n");
            return; // Skip this spawn if texture assignment fails
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
        }
    }
}