#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>
#include "SDL_Utils/sdl_utils.h"
#include "Plane/plane.h"
#include "Bullet/bullet.h"
#include "Enemy/enemy.h"
#include "Game/game.h"
#include "Interface/interface.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *tex = NULL;

    // Initialize SDL and create the window and renderer
    initSDL(&window, &renderer);

    // Create a plane object
    Plane plane = {SCREEN_WIDTH / 2 - plane.w, SCREEN_HEIGHT / 2 - 10, {255, 0, 0, 255}, NULL};

    // Load the plane image (texture)
    plane.texture = loadImage(renderer, "images/image.png", &plane.w, &plane.h);

    SDL_Texture *bg = loadImage(renderer, "images/backgrounds/1.png", NULL, NULL);
    SDL_Texture *bg_new = loadImage(renderer, "images/backgrounds/2.png", NULL, NULL);

    Uint32 startTime = SDL_GetTicks(); // Record the time when the game starts
    int transitioning = 0;             // Flag to indicate if a transition is happening
    int alpha = 0;

    Explosion explosionArray[100];
    int explosionArrayCount = 0;

    Enemy enemies[100];
    int enemyCount = 0;
    const int maxEnemies = 100;

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

        static int frameCount = 0;
        frameCount++;

        if (frameCount % 120 == 0) // Spawn an enemy every 2 seconds (assuming 60 FPS)
        {
            spawnEnemy(enemies, &enemyCount, maxEnemies, renderer);
        }

        updateExplosion(explosionArray, &explosionArrayCount, 12);

        /* ===== TRANSITION =====*/

        // Render the background using the new renderBg function
        // Render the initial background (bg) first
        renderBg(renderer, bg, bg_new, &transitioning, &startTime, &alpha);

        /* ===== TRANSITION =====*/

        renderExplosion(renderer, explosionArray, &explosionArrayCount);

        // Render the plane and enemy
        renderPlane(renderer, &plane);

        updateEnemies(enemies, &enemyCount);
        renderEnemies(renderer, enemies, enemyCount);

        updateBullets(bullets, &bulletCount);
        renderBullets(renderer, bullets, &bulletCount);

        checkCollision(bullets, &bulletCount, enemies, &enemyCount, explosionArray, &explosionArrayCount, renderer);

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
