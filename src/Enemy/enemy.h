#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "../Bullet/bullet.h"
#include "../SDL_Utils/sdl_utils.h"
#include "../Interface/interface.h"

typedef struct
{
    int health;
    int x, y;        // Position of the enemy
    SDL_Color color; // Color of the enemy
    SDL_Texture *texture;
    SDL_Rect hitbox;
    int w, h;
} Enemy;
void spawnEnemy(Enemy *enemies, int *enemyCount, int maxEnemies, SDL_Renderer *renderer);
void renderEnemies(SDL_Renderer *renderer, Enemy *enemies, int enemyCount);
void updateEnemies(Enemy *enemies, int *enemyCount, int *score);

#endif // ENEMY_H