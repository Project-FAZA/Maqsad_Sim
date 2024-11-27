#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "SDL_Utils/sdl_utils.h"
#include "Plane/plane.h"
#include "Bullet/bullet.h"
#include "Enemy/enemy.h"
#include "Game/game.h"
#include "Interface/interface.h"
#include "Leaderboard/leaderboard.h"

void playModeStage(
    SDL_Renderer *renderer,
    Plane *plane,
    Enemy enemies[],
    int *enemyCount,
    const int maxEnemies,
    Explosion explosionArray[],
    int *explosionArrayCount,
    Bullet bullets[],
    int *bulletCount,
    int *score,
    GameState *gameState, char username[4], int *charCount)
{
    // Handle plane movement input
    handleInput(plane, bullets, bulletCount);

    static int frameCount = 0;
    frameCount++;

    if (frameCount % (90) == 0) // Spawn an enemy every 2 seconds (assuming 60 FPS)
    {
        spawnEnemy(enemies, enemyCount, maxEnemies, renderer);
    }

    updateExplosion(explosionArray, explosionArrayCount, 12);

    if ((*score < 0) || (plane->health <= 0))
    {
        updateLeaderboard(username, *score);
        displayLeaderboard();
        (*gameState) = GAMEOVER;
        clearContents(plane,
                      enemies,
                      enemyCount,
                      maxEnemies,
                      explosionArray,
                      explosionArrayCount,
                      bullets,
                      bulletCount, score, gameState, username, charCount);
    }

    renderExplosion(renderer, explosionArray, explosionArrayCount);

    // Render the plane and enemy
    renderPlane(renderer, plane);

    updateEnemies(enemies, enemyCount, score);
    renderEnemies(renderer, enemies, *enemyCount);

    updateBullets(bullets, bulletCount);
    renderBullets(renderer, bullets, bulletCount);

    checkCollision(bullets, bulletCount, plane, enemies, enemyCount, explosionArray, explosionArrayCount, renderer, score);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *tex = NULL;

    // Initialize SDL and create the window and renderer
    initSDL(&window, &renderer);

    // Create a plane object
    Plane plane =
        {.y = SCREEN_HEIGHT / 2 - 10,
         .color = {255, 0, 0, 255},
         .health = 10};

    // Load the plane image (texture)
    plane.texture = loadImage(renderer, "images/plane.png", &plane.w, &plane.h);

    plane.x = SCREEN_WIDTH / 2 - plane.w;

    Explosion explosionArray[100];
    int explosionArrayCount = 0;

    Enemy enemies[100];
    int enemyCount = 0;
    const int maxEnemies = 100;

    Bullet bullets[100]; // Array to hold up to 100 bullets
    int bulletCount = 0; // Keep track of the number of bullets

    char username[4] = "";
    int charCount = 0;
    int score = 0;

    // Main game loop
    GameState gameState = MENU;
    SDL_Event e;

    while (gameState != QUIT)
    {
        // Handle events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                gameState = 3;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (gameState == MENU)
            menuMode(renderer, &gameState);
        else if (gameState == NAME)
            nameMode(renderer, &gameState, username, &charCount);
        else if (gameState == PLAYING)
        {
            playModeStage(renderer,
                          &plane,
                          enemies,
                          &enemyCount,
                          maxEnemies,
                          explosionArray,
                          &explosionArrayCount,
                          bullets,
                          &bulletCount, &score, &gameState, username, &charCount);
            playMode(renderer, score, username, plane.health);
        }
        else if (gameState == CREDITS)
        {
            creditsMode(renderer, &gameState);
        }
        else if (gameState == GAMEOVER)
        {
            gameOverMode(renderer, score, &gameState);
        }

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
