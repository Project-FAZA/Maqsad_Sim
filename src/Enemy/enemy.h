#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include "../Bullet/bullet.h"

typedef struct
{
    int x, y;        // Position of the enemy
    SDL_Color color; // Color of the enemy
} Enemy;

void renderEnemy(SDL_Renderer *renderer, Enemy *enemy);
void checkCollision(Bullet bullets[], int bulletCount, Enemy *enemy);

#endif // ENEMY_H