#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../Enemy/enemy.h"
#include "../Bullet/bullet.h"
#include "../Interface/interface.h"

void checkCollision(Bullet bullets[], int *bulletCount, Enemy *enemies, int *enemyCount, Explosion *explosionArray, int *explosionArrayCount, SDL_Renderer *renderer);

#endif
