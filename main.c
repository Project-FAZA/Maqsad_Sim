#include <SDL2/SDL.h>
#include <stdio.h>

// Define the screen width and height
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BULLET_SPEED 10

// Plane structure to hold the plane's position
typedef struct
{
    int x, y;        // Position of the plane
    SDL_Color color; // Color of the plane (for simplicity, using a solid color)
} Plane;

typedef struct
{
    int x, y;        // Position of the bullet
    int w, h;        // Width and height of the bullet
    SDL_Color color; // Color of the bullet
} Bullet;

void initSDL(SDL_Window **window, SDL_Renderer **renderer)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
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

void renderBullets(SDL_Renderer *renderer, Bullet *bullets, int *bulletCount)
{
    for (int i = 0; i < *bulletCount; ++i)
    {
        SDL_SetRenderDrawColor(renderer, bullets[i].color.r, bullets[i].color.g, bullets[i].color.b, bullets[i].color.a);
        SDL_Rect bulletRect = {bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h};
        SDL_RenderFillRect(renderer, &bulletRect);
    }
}

// Update the bullets (move them upwards)
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

// Shoot a bullet
void shootBullet(Plane *plane, Bullet *bullets, int *bulletCount)
{
    if (*bulletCount < 100)
    {                                                                                     // Limit the number of bullets
        Bullet newBullet = {plane->x + 25 / 2, plane->y - 25, 5, 20, {255, 255, 0, 255}}; // Yellow bullet
        bullets[*bulletCount] = newBullet;
        (*bulletCount)++;
    }
}

void renderPlane(SDL_Renderer *renderer, Plane *plane)
{
    // Set the drawing color for the plane
    SDL_SetRenderDrawColor(renderer, plane->color.r, plane->color.g, plane->color.b, plane->color.a);

    // Draw the body of the plane (a rectangle)
    SDL_Rect body = {plane->x, plane->y, 25, 100}; // Body is a horizontal rectangle
    SDL_RenderFillRect(renderer, &body);

    SDL_Rect wings = {plane->x - (75 / 2), plane->y + 40, 100, 20}; // Body is a horizontal rectangle
    SDL_RenderFillRect(renderer, &wings);

    SDL_Rect tail = {plane->x - (25 / 2), plane->y + 100, 50, 20}; // Body is a horizontal rectangle
    SDL_RenderFillRect(renderer, &tail);
}

void handleInput(Plane *plane)
{
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    // Move the plane with arrow keys
    if (keystates[SDL_SCANCODE_UP])
    {
        plane->y -= 5; // Move up
    }
    if (keystates[SDL_SCANCODE_DOWN])
    {
        plane->y += 5; // Move down
    }
    if (keystates[SDL_SCANCODE_LEFT])
    {
        plane->x -= 5; // Move left
    }
    if (keystates[SDL_SCANCODE_RIGHT])
    {
        plane->x += 5; // Move right
    }
}

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize SDL and create the window and renderer
    initSDL(&window, &renderer);

    // Create a plane object
    Plane plane = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 10, {255, 0, 0, 255}}; // Red plane

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

        // Render the plane
        renderPlane(renderer, &plane);

        updateBullets(bullets, &bulletCount);
        renderBullets(renderer, bullets, &bulletCount);

        // Present the rendered frame
        SDL_RenderPresent(renderer);

        // Delay to keep the frame rate consistent
        SDL_Delay(16); // Roughly 60 FPS
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
