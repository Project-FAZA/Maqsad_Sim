#include "sdl_utils.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        printf("Error SDL2_image Initialization");
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    // Create window
    *window = SDL_CreateWindow("SDL2 Plane", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Create renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void handleInput(Plane *plane, Bullet *bullets, int *bulletCount)
{
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_UP])
        plane->y -= 7;
    if (keystates[SDL_SCANCODE_DOWN])
        plane->y += 7;
    if (keystates[SDL_SCANCODE_LEFT])
        plane->x -= 7;
    if (keystates[SDL_SCANCODE_RIGHT])
        plane->x += 7;

    if (plane->x > SCREEN_WIDTH)
        plane->x = 0 - plane->w;
    else if (plane->x < 0 - plane->w)
        plane->x = SCREEN_WIDTH;

    if (plane->y < 0)
        plane->y = 0;
    else if (plane->y > SCREEN_HEIGHT - plane->h)
        plane->y = SCREEN_HEIGHT - plane->h;

    static int spaceKeyPressed = 0; // Prevent rapid fire without deliberate key press
    if (keystates[SDL_SCANCODE_SPACE])
    {
        if (!spaceKeyPressed)
        {
            shootBullet(plane, bullets, bulletCount);
            spaceKeyPressed = 1;
        }
    }
    else
    {
        spaceKeyPressed = 0; // Reset flag when the space bar is released
    }
}

void getUsername(char username[4], SDL_Renderer *renderer, int *charCount)
{
    // Get the current state of all keys
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    static int keyPressed[SDL_NUM_SCANCODES] = {0};

    // Handle backspace (if backspace is pressed and there's at least one character)
    if (keystates[SDL_SCANCODE_BACKSPACE])
    {
        if (!keyPressed[SDL_SCANCODE_BACKSPACE] && *charCount > 0)
        {
            // Remove the last character
            username[--(*charCount)] = '\0';
            keyPressed[SDL_SCANCODE_BACKSPACE] = 1;
        }
    }
    else
    {
        keyPressed[SDL_SCANCODE_BACKSPACE] = 0;
    }

    // Handle alphanumeric character input (only A-Z and a-z)
    if (*charCount < 3)
    {
        for (char key = 'A'; key <= 'Z'; key++) // Check for letters A-Z
        {
            SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
            if (keystates[scancode])
            {
                if (!keyPressed[scancode])
                {
                    // Add the uppercase letter to the username
                    username[(*charCount)++] = key;
                    username[*charCount] = '\0'; // Null-terminate the string
                    keyPressed[scancode] = 1;
                    break; // Exit the loop once we have processed a key
                }
            }
            else
            {
                keyPressed[scancode] = 0;
            }
        }

        for (char key = 'a'; key <= 'z'; key++) // Check for letters a-z
        {
            SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
            if (keystates[scancode])
            {
                if (!keyPressed[scancode])
                {
                    // Add the uppercase letter to the username
                    username[(*charCount)++] = key - 32;
                    username[*charCount] = '\0'; // Null-terminate the string
                    keyPressed[scancode] = 1;
                    break; // Exit the loop once we have processed a key
                }
            }
            else
            {
                keyPressed[scancode] = 0;
            }
        }
    }
}

void getMenuOpt(SDL_Renderer *renderer, int *selected)
{
    // Get the current state of all keys
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    static int keyPressed[SDL_NUM_SCANCODES] = {0};

    if (keystates[SDL_SCANCODE_UP])
    {
        if (!keyPressed[SDL_SCANCODE_UP])
        {
            (*selected)--;
            keyPressed[SDL_SCANCODE_UP] = 1;
        }
    }
    else
    {
        keyPressed[SDL_SCANCODE_UP] = 0;
    }

    if (keystates[SDL_SCANCODE_DOWN])
    {
        if (!keyPressed[SDL_SCANCODE_DOWN])
        {
            (*selected)++;
            keyPressed[SDL_SCANCODE_DOWN] = 1;
        }
    }
    else
    {
        keyPressed[SDL_SCANCODE_DOWN] = 0;
    }

    if ((*selected) > 2)
        *selected = 0;
    if ((*selected) < 0)
        *selected = 2;
}