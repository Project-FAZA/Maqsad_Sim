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
#include "Building/building.h"
#include "Game/game.h"
#include "Interface/interface.h"
#include "Leaderboard/leaderboard.h"

void playModeStage(
    SDL_Renderer *renderer,
    Plane *plane,
    Enemy enemies[],
    Building buildings[],
    int *enemyCount,
    int *buildingCount,
    const int maxEnemies,
    const int maxBuildings,
    Explosion explosionArray[],
    int *explosionArrayCount,
    Bullet bullets[],
    int *bulletCount,
    GameState *gameState, char username[4], int *charCount)
{
    if (!Mix_PlayingMusic())
        playMusic('B');
    // Handle plane movement input
    handleInput(plane, bullets, bulletCount);

    static int frameCount = 0;
    frameCount++;

    if (frameCount % (90) == 0) // Spawn an enemy every 1.5 seconds (assuming 60 FPS)
        spawnEnemy(enemies, enemyCount, maxEnemies, renderer);
    if (frameCount % (5 * 60) == 0)
        spawnBuilding(buildings, buildingCount, maxBuildings, renderer);

    updateExplosion(explosionArray, explosionArrayCount, 12);

    if ((plane->score.base < 0) || (plane->health <= 0))
    {
        int total = plane->score.base + plane->score.adults + (plane->score.elders * 0.5) + (plane->score.children * 2);
        updateLeaderboard(username, total);
        displayLeaderboard();
        Mix_HaltMusic();
        playSoundEffect('L');
        SDL_Delay(1200);
        playMusic('G');
        SDL_Delay(4000);
        (*gameState) = GAMEOVER;
        clearContents(plane,
                      enemies, enemyCount,
                      buildings, buildingCount,
                      maxEnemies, maxBuildings,
                      explosionArray,
                      explosionArrayCount,
                      bullets,
                      bulletCount, gameState, username, charCount);
    }

    renderExplosion(renderer, explosionArray, explosionArrayCount);

    // Render the plane and enemy
    renderPlane(renderer, plane);

    updateEnemies(enemies, enemyCount, &plane->score.base);
    renderEnemies(renderer, enemies, *enemyCount);

    updateBuildings(buildings, buildingCount, &plane->score.base);
    renderBuildings(renderer, buildings, *buildingCount);

    updateBullets(bullets, bulletCount);
    renderBullets(renderer, bullets, bulletCount);

    checkCollision(bullets, bulletCount, plane, enemies, enemyCount, buildings, buildingCount, explosionArray, explosionArrayCount, renderer);
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
         .health = 10,
         .score = {0, 0, 0, 0}};

    // Load the plane image (texture)
    plane.texture = loadImage(renderer, "images/plane.png", &plane.w, &plane.h);

    plane.x = SCREEN_WIDTH / 2 - plane.w;

    Explosion explosionArray[100];
    int explosionArrayCount = 0;

    Enemy enemies[100];
    int enemyCount = 0;
    const int maxEnemies = 100;

    Building buildings[100];
    int buildingCount = 0;
    const int maxBuildings = 100;

    Bullet bullets[100]; // Array to hold up to 100 bullets
    int bulletCount = 0; // Keep track of the number of bullets

    char username[4] = "";
    int charCount = 0;

    // Main game loop
    GameState gameState = MENU;
    SDL_Event e;

    static char nameInLeaderboard[4] = "";
    static int clear = 0;
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
        {
            menuMode(renderer, &gameState);
        }
        else if (gameState == NAME)
            nameMode(renderer, &gameState, username, &charCount);
        else if (gameState == PLAYING)
        {
            playModeStage(renderer,
                          &plane,
                          enemies,
                          buildings,
                          &enemyCount,
                          &buildingCount,
                          maxEnemies,
                          maxBuildings,
                          explosionArray,
                          &explosionArrayCount,
                          bullets,
                          &bulletCount, &gameState, username, &charCount);
            playMode(renderer, plane.score.base, username, plane.health);
        }
        else if (gameState == CREDITS)
        {
            creditsMode(renderer, &gameState);
        }
        else if (gameState == GAMEOVER)
        {
            gameOverMode(renderer, plane.score, &gameState);
            if (gameState != GAMEOVER)
            {
                plane.score.base = 0;
                plane.score.adults = 0;
                plane.score.elders = 0;
                plane.score.children = 0;
            }
        }
        else if (gameState == LEADERBOARD)
        {
            leaderboardMode(renderer, &gameState);
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
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
