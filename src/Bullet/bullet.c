#include "bullet.h"

void renderBullets(SDL_Renderer *renderer, Bullet *bullets, int *bulletCount)
{
    for (int i = 0; i < *bulletCount; ++i)
    {
        SDL_SetRenderDrawColor(renderer, bullets[i].color.r, bullets[i].color.g, bullets[i].color.b, bullets[i].color.a);
        SDL_Rect bulletRect = {bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h};
        SDL_RenderFillRect(renderer, &bulletRect);
    }
}

void updateBullets(Bullet *bullets, int *bulletCount)
{
    for (int i = 0; i < *bulletCount; ++i)
    {
        bullets[i].y -= BULLET_SPEED; // Move the bullet upwards
    }

    // Remove bullets that have gone off-screen
    for (int i = 0; i < *bulletCount; ++i)
    {
        if (bullets[i].y < 0)
        {
            for (int j = i; j < *bulletCount - 1; ++j)
            {
                bullets[j] = bullets[j + 1]; // Shift bullets down
            }
            (*bulletCount)--; // Decrease the bullet count
            i--;              // Adjust the index
        }
    }
}

void shootBullet(Plane *plane, Bullet *bullets, int *bulletCount)
{
    if (*bulletCount < 100) // Limit the number of bullets
    {
        Bullet newBullet = {plane->x + 50 / 2, plane->y - 25, 5, 20, {255, 255, 0, 255}}; // Yellow bullet
        bullets[*bulletCount] = newBullet;
        (*bulletCount)++;
    }
}