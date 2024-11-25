#include "enemy.h"

void spawnEnemy(Enemy *enemies, int *enemyCount, int maxEnemies, SDL_Renderer *renderer)
{
    if ((*enemyCount) < maxEnemies)
    {
        int x = rand() % (SCREEN_WIDTH - 50); // Random x between 0 and SCREEN_WIDTH - 50

        enemies[*enemyCount].texture = loadImage(renderer, "images/destroyer.png", &enemies[*enemyCount].w, &enemies[*enemyCount].h);

        if (!enemies[*enemyCount].texture)
        {
            printf("Failed to load enemy texture\n");
            return; // Skip this spawn if texture loading fails
        }

        enemies[*enemyCount].x = x;
        enemies[*enemyCount].y = 0;

        enemies[*enemyCount].health = 10;
        (*enemyCount)++;
    }
}

void renderEnemies(SDL_Renderer *renderer, Enemy *enemies, int enemyCount)
{
    for (int i = 0; i < enemyCount; i++)
    {
        if (enemies[i].health <= 0)
        {

            continue; // Skip dead enemies
        }

        SDL_Rect body = {enemies[i].x, enemies[i].y, 50, 50};
        enemies[i].hitbox = body;

        SDL_RenderCopy(renderer, enemies[i].texture, NULL, &(enemies[i].hitbox));
    }
}

void updateEnemies(Enemy *enemies, int *enemyCount, int *score)
{
    int multiplier = 2 + ((*score) / 100); // multiplier effect after every 100 points

    for (int i = 0; i < *enemyCount; i++)
    {

        enemies[i].y += 1 * multiplier;

        // Remove enemies that go off-screen
        if (enemies[i].y > SCREEN_HEIGHT)
        {
            // Shift the array to remove the enemy
            for (int j = i; j < (*enemyCount) - 1; j++)
            {
                enemies[j] = enemies[j + 1];
            }
            (*enemyCount)--;
            i--; // Adjust index to recheck shifted enemy
            (*score) -= 5;
        }
    }
}