#include "game.h"

void killEnemy(int *i, Enemy *enemies, int *enemyCount, Explosion *explosionArray, int *explosionArrayCount, SDL_Renderer *renderer, int *score)
{
    printf("Enemy destroyed!\n");
    (*score) += 10;
    // Spawn explosion at enemy's position
    loadExplosion(explosionArray, explosionArrayCount, renderer, "images/explosion/HD", 12, enemies[*i].x, enemies[*i].y);

    // Remove the enemy by shifting the array
    for (int k = (*i); k < *enemyCount - 1; k++)
    {
        enemies[k] = enemies[k + 1];
    }
    (*enemyCount)--; // Decrease enemy count
    (*i)--;          // Adjust index to account for removed enemy
}

void checkCollision(Bullet bullets[], int *bulletCount, Plane *plane, Enemy *enemies, int *enemyCount, Explosion *explosionArray, int *explosionArrayCount, SDL_Renderer *renderer, int *score)
{
    for (int i = 0; i < *enemyCount; i++) // Iterate through enemies
    {
        for (int j = 0; j < *bulletCount; j++) // Iterate through bullets
        {
            SDL_Rect bulletRect = {bullets[j].x, bullets[j].y, bullets[j].w, bullets[j].h};

            if (SDL_HasIntersection(&bulletRect, &enemies[i].hitbox))
            {
                // Enemy hit: decrease health
                enemies[i].health -= 10;
                printf("Enemy %d HIT! Health: %d\n", i, enemies[i].health);

                // Remove bullet by shifting the array
                for (int k = j; k < *bulletCount - 1; k++)
                {
                    bullets[k] = bullets[k + 1];
                }
                (*bulletCount)--; // Decrease bullet count
                j--;              // Adjust index to account for removed bullet

                // If enemy is dead, remove it
                if (enemies[i].health <= 0)
                {
                    killEnemy(&i, enemies, enemyCount, explosionArray, explosionArrayCount, renderer, score);

                    break; // Exit bullet loop for this enemy
                }
            }
        }
        if (SDL_HasIntersection(&enemies[i].hitbox, &(plane->hitbox)))
        {
            killEnemy(&i, enemies, enemyCount, explosionArray, explosionArrayCount, renderer, score);
            plane->health -= 10;
        }
    }
}

void clearContents(Plane *plane,
                   Enemy enemies[], int *enemyCount,
                   const int maxEnemies,
                   Explosion explosionArray[],
                   int *explosionArrayCount,
                   Bullet bullets[],
                   int *bulletCount,
                   int *score,
                   GameState *gameState, char username[4], int *charCount)
{
    *enemyCount = 0;
    *explosionArrayCount = 0;
    *bulletCount = 0;
    *score = 0;                             // Reset score if needed
    plane->health = 10;                     // Reset health to the starting value
    plane->x = SCREEN_WIDTH / 2 - plane->w; // Reset plane position (centered)
    plane->y = SCREEN_HEIGHT / 2 - 10;      // Reset plane's vertical position

    // Clear the enemies array by resetting all elements to their default state
    memset(enemies, 0, sizeof(Enemy) * maxEnemies);

    // Clear the explosions array by resetting all elements to their default state
    memset(explosionArray, 0, sizeof(Explosion) * 100);

    // Clear the bullets array by resetting all elements to their default state
    memset(bullets, 0, sizeof(Bullet) * 100);

    strcpy(username, "");

    *charCount = 0;
}