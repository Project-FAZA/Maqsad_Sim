#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>
#include "../Plane/plane.h"

#define BULLET_SPEED 10

typedef struct
{
    int x, y;        // Position of the bullet
    int w, h;        // Width and height of the bullet
    SDL_Color color; // Color of the bullet
} Bullet;

void renderBullets(SDL_Renderer *renderer, Bullet *bullets, int *bulletCount);
void updateBullets(Bullet *bullets, int *bulletCount);
void shootBullet(Plane *plane, Bullet *bullets, int *bulletCount);

#endif // BULLET_H