#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "SDL_Utils/sdl_utils.h"
#include "Plane/plane.h"
#include "Bullet/bullet.h"
#include "Enemy/enemy.h"

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *tex = NULL;

    // Initialize SDL and create the window and renderer
    initSDL(&window, &renderer);

    // Create a plane object
    Plane plane = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 10, {255, 0, 0, 255}, NULL};

    // Load the plane image (texture)
    plane.texture = load_image(renderer, "images/image.png");

    Enemy enemy = {0, 0, {255, 255, 0, 255}};
    Bullet bullets[100]; // Array to hold up to 100 bullets
    int bulletCount = 0; // Keep track of the number of bullets

    // Main game loop
    int running = 1;
    SDL_Event e;
    while (running)
    {
        // Handle events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                running = 0;
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
            {
                shootBullet(&plane, bullets, &bulletCount);
            }
        }

        // Handle plane movement input
        handleInput(&plane);

        // Clear the screen (fill with black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background color (black)
        SDL_RenderClear(renderer);

        // Render the plane and enemy
        SDL_Rect plane_rect = {plane.x, plane.y, 50, 50};
        SDL_RenderCopy(renderer, plane.texture, NULL, &plane_rect);

        renderEnemy(renderer, &enemy);
        updateBullets(bullets, &bulletCount);
        renderBullets(renderer, bullets, &bulletCount);
        checkCollision(bullets, bulletCount, &enemy);

        // Present the rendered frame
        SDL_RenderPresent(renderer);

        // Delay to keep the frame rate consistent
        SDL_Delay(16); // Roughly 60 FPS
    }

    // Clean up and quit SDL
    SDL_DestroyTexture(plane.texture); // Free the texture once done
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
