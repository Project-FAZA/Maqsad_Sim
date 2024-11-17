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

void renderBg(SDL_Renderer *renderer, SDL_Texture *bg)
{
    SDL_RenderCopy(renderer, bg, NULL, &((SDL_Rect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}));
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color)
{
    if (SDL_strlen(text) == 0)
        return;
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface)
    {
        printf("Failed to create text surface! TTF_Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        printf("Failed to create text texture! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void menuMode(SDL_Renderer *renderer, GameState *gameState)
{
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    static int toggle = 0;

    char *opts[] = {"PLAY", "CREDITS", "QUIT"};
    static int selected = 0;

    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color amberColor = {255, 191, 0, 255};

    TTF_Font *mainFont = TTF_OpenFont("fonts/alpha-taurus-font/Main.ttf", 100);
    TTF_Font *font = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 30);

    renderText(renderer, mainFont, "MAQSAD", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 200, toggle ? amberColor : whiteColor);
    toggle = !toggle;

    getMenuOpt(renderer, &selected);

    for (int i = 0; i < 3; i++)
    {
        if (i == selected)
        {
            renderText(renderer, font, ">", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25 + 65 * i, toggle ? amberColor : whiteColor);
            renderText(renderer, font, opts[i], SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 25 + 65 * i, toggle ? amberColor : whiteColor);
        }
        else
            renderText(renderer, font, opts[i], SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 25 + 65 * i, whiteColor);
    }

    if (keystates[SDL_SCANCODE_RETURN])
    {
        switch (selected)
        {
        case 0:
            *gameState = NAME;
            break;
        case 2:
            *gameState = QUIT;
        default:
            break;
        }
    }
}

void nameMode(SDL_Renderer *renderer, GameState *gameState, char username[4], int *charCount)
{
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    static int toggle = 0;
    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color amberColor = {255, 191, 0, 255};

    getUsername(username, renderer, charCount);

    TTF_Font *font = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 30);

    renderText(renderer, font, "Enter your name: ", SCREEN_WIDTH / 2 - 400, SCREEN_HEIGHT / 2 - 20, whiteColor);
    renderText(renderer, font, username, SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 20, toggle ? amberColor : whiteColor);
    renderText(renderer, font, "___", SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 10, whiteColor);

    toggle = !toggle;
    if (keystates[SDL_SCANCODE_RETURN] && (*charCount == 3))
        (*gameState) = PLAYING;
}

void playMode(SDL_Renderer *renderer, int score, char username[4])
{
    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color amberColor = {255, 191, 0, 255};

    TTF_Font *font = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 30);

    renderText(renderer, font, username, 10, 10, whiteColor);

    int w, h;

    char str[100] = "0";
    static int prevScore = 0;
    sprintf(str, "%d", score);

    TTF_SizeText(font, str, &w, &h);
    renderText(renderer, font, str, SCREEN_WIDTH - w - 10, 10, (prevScore != score) ? amberColor : whiteColor); // Gives a glowy effect whenever the score changes
    prevScore = score;
}