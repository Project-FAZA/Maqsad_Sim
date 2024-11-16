#include "interface.h"

SDL_Texture *loadImage(SDL_Renderer *renderer, const char *imagePath, int *w, int *h)
{
    // Load the image as an SDL_Surface
    SDL_Surface *surface = IMG_Load(imagePath);
    if (surface == NULL)
    {
        printf("Error loading image: %s\n", IMG_GetError());
        return NULL; // Return NULL if loading fails
    }

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (w)
        (*w) = surface->w;
    if (h)
        (*h) = surface->h;

    SDL_FreeSurface(surface); // Free the surface after creating the texture

    if (texture == NULL)
    {
        printf("Error creating texture: %s\n", SDL_GetError());
    }

    return texture;
}

void loadExplosion(Explosion *explosionArray, int *explosionArrayCount, SDL_Renderer *renderer, const char *folderPath, int totalFrames, int x, int y)
{
    // Ensure we don't exceed the array size (let's assume max 100 explosions)
    if (*explosionArrayCount >= 100)
        return;

    Explosion *newExplosion = &explosionArray[*explosionArrayCount];
    // Load all frames for the explosion animation
    for (int i = 0; i < totalFrames; i++)
    {
        char filename[100];
        snprintf(filename, sizeof(filename), "%s/%d.png", folderPath, i + 1);
        newExplosion->texture[i] = loadImage(renderer, filename, NULL, NULL);
    }
    // Initialize the current frame to 0 (first frame of animation)
    newExplosion->currFrame = 0;

    // Set explosion position to passed x and y
    newExplosion->x = x;
    newExplosion->y = y;

    newExplosion->lastFrameTime = SDL_GetTicks();

    // Increase the explosion count to track the new explosion
    (*explosionArrayCount)++;
}

void updateExplosion(Explosion *explosionArray, int *explosionArrayCount, int totalFrames)
{
    for (int i = 0; i < *explosionArrayCount; i++)
    {
        // Track the time passed since the last frame update for this specific explosion
        Uint32 currentTime = SDL_GetTicks();

        // If it's time for the next frame (every 50ms for frames)
        if (currentTime - explosionArray[i].lastFrameTime >= 50)
        {
            // Update the last frame time to the current time
            explosionArray[i].lastFrameTime = currentTime;

            // Move to the next frame of the explosion
            explosionArray[i].currFrame++;

            // If the current frame exceeds the total number of frames, remove the explosion
            if (explosionArray[i].currFrame >= totalFrames)
            {
                // Remove explosion after the last frame
                for (int j = i; j < *explosionArrayCount - 1; j++)
                {
                    explosionArray[j] = explosionArray[j + 1]; // Shift remaining explosions down
                }
                (*explosionArrayCount)--; // Decrease explosion count
                i--;                      // Adjust index since the array shifted
            }
        }
    }
}

void renderExplosion(SDL_Renderer *renderer, Explosion *explosionArray, int *explosionArrayCount)
{
    for (int i = 0; i < *explosionArrayCount; i++)
    {
        // Get the current explosion texture (frame) based on currFrame
        SDL_Texture *currentTexture = explosionArray[i].texture[explosionArray[i].currFrame];

        // Render the current explosion frame at the explosion's position (x, y)
        SDL_Rect destRect = {explosionArray[i].x, explosionArray[i].y, 50, 50}; // Adjust size as necessary
        SDL_RenderCopy(renderer, currentTexture, NULL, &destRect);
    }
}

void transitionBackground(SDL_Renderer *renderer, SDL_Texture *background1, SDL_Texture *background2, int alpha)
{
    SDL_SetTextureBlendMode(background2, SDL_BLENDMODE_BLEND);

    // Render the first (old) background
    SDL_RenderCopy(renderer, background1, NULL, NULL);

    // Set the alpha value for the second (new) background
    SDL_SetTextureAlphaMod(background2, alpha);

    // Render the second (new) background with the current alpha
    SDL_RenderCopy(renderer, background2, NULL, NULL);
}

void renderBg(SDL_Renderer *renderer, SDL_Texture *bg, SDL_Texture *bg_new, int *transitioning, Uint32 *startTime, int *alpha)
{
    // If no transition is happening yet, render the initial background (bg)
    if (!(*transitioning))
    {
        SDL_RenderCopy(renderer, bg, NULL, &((SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}));
    }

    // If 5 seconds have passed, start the background transition
    if (SDL_GetTicks() - *startTime >= 5000 && !(*transitioning))
    {
        *transitioning = 1;          // Start the transition
        *startTime = SDL_GetTicks(); // Reset the timer after starting the transition
    }

    // If transitioning, update the alpha value
    if (*transitioning)
    {
        // Calculate how much time has passed
        Uint32 elapsedTime = SDL_GetTicks() - *startTime;

        // If the transition should be happening within the 2-second period
        if (elapsedTime <= 2000)
        {
            // Calculate the alpha value based on the time passed
            *alpha = (255 * elapsedTime) / 2000; // Gradual transition
        }
        else
        {
            *alpha = 255; // Fully transition to new background
        }

        // Render the background transition
        transitionBackground(renderer, bg, bg_new, *alpha);
    }
}
