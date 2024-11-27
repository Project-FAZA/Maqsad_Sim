#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../Enemy/enemy.h"
#include "../Building/building.h"
#include "../Bullet/bullet.h"
#include "../Interface/interface.h"
#include "../Music/music.h"

void checkCollision(Bullet bullets[], int *bulletCount, Plane *plane, Enemy *enemies, int *enemyCount, Building *buildings, int *buildingsCount, Explosion *explosionArray, int *explosionArrayCount, SDL_Renderer *renderer, int *score);
void clearContents(Plane *plane,
                   Enemy enemies[], int *enemyCount,
                   Building buildings[], int *buildingCount,
                   const int maxEnemies,
                   const int maxBuildings,
                   Explosion explosionArray[],
                   int *explosionArrayCount,
                   Bullet bullets[],
                   int *bulletCount,
                   int *score,
                   GameState *gameState, char username[4], int *charCount);
#endif
