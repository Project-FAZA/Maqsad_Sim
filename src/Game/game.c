#include "game.h"

void checkCollision(Bullet bullets[], int *bulletCount, Enemy *enemies, int *enemyCount, Explosion *explosionArray, int *explosionArrayCount, SDL_Renderer *renderer, int *score)
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
                    printf("Enemy %d destroyed!\n", i);
                    (*score) += 10;
                    // Spawn explosion at enemy's position
                    loadExplosion(explosionArray, explosionArrayCount, renderer, "images/explosion/HD", 12, enemies[i].x, enemies[i].y);

                    // Remove the enemy by shifting the array
                    for (int k = i; k < *enemyCount - 1; k++)
                    {
                        enemies[k] = enemies[k + 1];
                    }
                    (*enemyCount)--; // Decrease enemy count
                    i--;             // Adjust index to account for removed enemy
                    break;           // Exit bullet loop for this enemy
                }
            }
        }
    }
}