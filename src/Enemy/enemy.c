#include "enemy.h"

void renderEnemy(SDL_Renderer *renderer, Enemy *enemy)
{
    SDL_SetRenderDrawColor(renderer, enemy->color.r, enemy->color.g, enemy->color.b, enemy->color.a);
    SDL_Rect body = {enemy->x, enemy->y, 50, 50};
    SDL_RenderFillRect(renderer, &body);
}

void checkCollision(Bullet bullets[], int bulletCount, Enemy *enemy)
{
    SDL_Rect body = {enemy->x, enemy->y, 50, 50};

    for (int i = 0; i < bulletCount; i++)
    {
        SDL_Rect bulletRect = {bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h};

        if (SDL_HasIntersection(&bulletRect, &body))
        {
            enemy->color.r = 255;
            enemy->color.g = 255;
            enemy->color.b = 255;
            enemy->color.a = 255;
        }
    }
}