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

void getCentreOfText(TTF_Font *font, char *text, int *x, int *y)
{
    int w, h;
    TTF_SizeText(font, text, &w, &h);

    if (x != NULL)
        (*x) = (SCREEN_WIDTH - w) / 2;
    if (y != NULL)
        (*y) = (SCREEN_HEIGHT - h) / 2;
}

void menuMode(SDL_Renderer *renderer, GameState *gameState)
{
    if (!Mix_PlayingMusic())
        playMusic('S');

    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    static int toggle = 0;

    char *opts[] = {"PLAY", "CREDITS", "QUIT"};
    static int selected = 0;

    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color amberColor = {255, 191, 0, 255};

    static TTF_Font *mainFont = NULL;
    static TTF_Font *font = NULL;

    if (!mainFont)
    {
        mainFont = TTF_OpenFont("fonts/alpha-taurus-font/Main.ttf", 100);
    }
    if (!font)
    {
        font = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 30);
    }

    int mx, my;
    getCentreOfText(mainFont, "MAQSAD", &mx, &my);

    renderText(renderer, mainFont, "MAQSAD", mx, my - 150, toggle ? amberColor : whiteColor);
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

    if (getButtonClick(SDL_SCANCODE_RETURN))
    {
        playSoundEffect('C');
        switch (selected)
        {
        case 0:
            *gameState = NAME;
            break;
        case 1:
            *gameState = CREDITS;
            break;
        case 2:
            *gameState = QUIT;
            break;
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

    static TTF_Font *font = NULL;
    if (!font)
    {
        font = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 30);
    }

    renderText(renderer, font, "Enter your name: ", SCREEN_WIDTH / 2 - 400, SCREEN_HEIGHT / 2 - 20, whiteColor);
    renderText(renderer, font, username, SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 20, toggle ? amberColor : whiteColor);
    renderText(renderer, font, "___", SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 10, whiteColor);

    toggle = !toggle;
    if (keystates[SDL_SCANCODE_RETURN] && (*charCount == 3))
    {
        (*gameState) = PLAYING;
        Mix_HaltMusic();
    }
}

void playMode(SDL_Renderer *renderer, int score, char username[4], int health)
{
    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color amberColor = {255, 191, 0, 255};

    static TTF_Font *font = NULL;
    if (!font)
    {
        font = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 30);
    }

    renderText(renderer, font, username, 10, 10, whiteColor);

    int w, h;

    char str[100] = "0";
    static int prevScore = 0;
    sprintf(str, "%d", score);

    TTF_SizeText(font, str, &w, &h);
    renderText(renderer, font, str, SCREEN_WIDTH - w - 10, 10, (prevScore != score) ? amberColor : whiteColor); // Gives a glowy effect whenever the score changes

    sprintf(str, "HP %d", health);

    TTF_SizeText(font, str, &w, &h);
    renderText(renderer, font, str, 10, 40, whiteColor);
    prevScore = score;
}

void creditsMode(SDL_Renderer *renderer, GameState *gameState)
{
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color amberColor = {255, 191, 0, 255};

    static int toggle = 0;
    static int j = 0;

    static TTF_Font *mainFont = NULL;
    static TTF_Font *font = NULL;
    static TTF_Font *creditFont = NULL;

    if (!mainFont)
    {
        mainFont = TTF_OpenFont("fonts/alpha-taurus-font/Main.ttf", 100);
    }
    if (!font)
    {
        font = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 30);
    }
    if (!creditFont)
    {
        creditFont = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 20);
    }

    const char credits[][300] = {
        "Maqsad Sim was designed, developed,",
        "and brought to life by our team.",
        "Inspired by the thrill of aerial combat",
        "and the challenge of navigating hostile skies,",
        "this top-down shooter is a labor of creativity",
        "and passion. Every pixel, line of code,",
        "and burst of sound was crafted to immerse",
        "players in high-octane fights and",
        "adrenaline-pumping action. Special thanks",
        "to Fadil Faisal, Ahmed Rashdi, Zain-ul Abideen,",
        "and Ahmed Raza for their encouragement and feedback",
        "throughout development. To all players, thank you for",
        "taking to the skies - your support fuels the journey.",
        "Fly high, shoot true, and enjoy the game!"};

    char titles[][100] = {"STRUCTURE AND UI/UX DESIGN", "PHYSICS AND GAMEPLAY MECHANICS", "IMAGE INTEGRATION AND OPTIMIZATION", "AUDIO DESIGN AND MUSIC COMPOSITION"};
    char names[][50] = {"Fadil Faisal", "Ahmed Rashdi", "Zain-ul-Abideen", "Ahmed Raza"};
    char rollNums[][50] = {"24K-0635", "24K-0709", "24K-0818", "24K-1010"};

    int y1 = SCREEN_HEIGHT / 2 - 200;
    int mx;
    getCentreOfText(mainFont, "CREDITS", &mx, NULL);

    for (int i = 0; i < 14; i++)
    {
        int y = SCREEN_HEIGHT / 2 - 25 + 50 * i;
        renderText(renderer, creditFont, credits[i], SCREEN_WIDTH / 2 - 500, y - j, (SDL_Color){179, 235, 242, 255});
    }

    renderText(renderer, mainFont, "CREDITS", mx, y1 - j, toggle ? amberColor : whiteColor);
    for (int i = 0; i < 4; i++)
    {
        int y = SCREEN_HEIGHT / 2 - 25 + 200 * i;
        renderText(renderer, font, titles[i], SCREEN_WIDTH / 2 - 500, y - j + 800, amberColor);
        renderText(renderer, font, names[i], SCREEN_WIDTH / 2 - 500, y - j + 100 + 800, whiteColor);
        renderText(renderer, font, rollNums[i], SCREEN_WIDTH - 500, y - j + 100 + 800, whiteColor);
    }

    renderText(renderer, font, "MENU", 100, SCREEN_HEIGHT - 50, toggle ? amberColor : whiteColor);

    renderText(renderer, font, ">", 50, SCREEN_HEIGHT - 50, toggle ? amberColor : whiteColor);

    toggle = !toggle;
    if (((SCREEN_HEIGHT / 2 - 25 + 200 * 5) - j + 100 + 800) > 0)
        j += 2;

    if (getCreditOpt())
    {
        (*gameState) = MENU;
        playSoundEffect('C');
        j = 0;
    }
}

void gameOverMode(SDL_Renderer *renderer, Score score, GameState *gameState)
{
    static int toggle = 0;
    SDL_Color color = {255, 255 * toggle, 255 * toggle, 255};
    SDL_Color whiteColor = {255, 255, 255, 255};
    static TTF_Font *mainFont = NULL;

    if (!mainFont)
    {
        mainFont = TTF_OpenFont("fonts/alpha-taurus-font/Main.ttf", 100);
    }

    static TTF_Font *font = NULL;
    if (!font)
    {
        font = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 30);
    }

    char *msg = "Game Over";

    int mx;
    getCentreOfText(mainFont, msg, &mx, NULL);
    renderText(renderer, mainFont, msg, mx, 50, color);
    toggle = !toggle;

    char scores[][50] = {"BASE:", "ADULTS:", "ELDERS:", "CHILDREN:"};
    int scoreNums[4] = {score.base, score.adults, score.elders, score.children};
    int total = score.base + score.adults + (score.elders * 0.5) + (score.children * 2);
    for (int i = 0; i < 4; i++)
    {
        int y = SCREEN_HEIGHT / 2 - 25 + 65 * i;
        renderText(renderer, font, scores[i], SCREEN_WIDTH / 2 - 200 - 75, y, whiteColor);

        char numStr[100] = "";
        char numStrBase[4][100] = {"%d", "%d", "%d x 0.5", "%d x 2"};

        sprintf(numStr, numStrBase[i], scoreNums[i]);
        renderText(renderer, font, numStr, SCREEN_WIDTH - 500 - 75, y, whiteColor);
    }
    renderText(renderer, font, "____________________", SCREEN_WIDTH / 2 - 200 - 75, SCREEN_HEIGHT / 2 - 25 + 65 * 3 + 20, whiteColor);
    renderText(renderer, font, "TOTAL: ", SCREEN_WIDTH / 2 - 200 - 75, SCREEN_HEIGHT / 2 - 25 + 65 * 4, whiteColor);

    char totalStr[100] = "";

    sprintf(totalStr, "%d", total);

    renderText(renderer, font, totalStr, SCREEN_WIDTH - 500 - 75, SCREEN_HEIGHT / 2 - 25 + 65 * 4, whiteColor);

    if (!Mix_PlayingMusic() && getButtonClick(SDL_SCANCODE_RETURN))
    {
        // gameOverAnim(renderer, score, 1);
        (*gameState) = LEADERBOARD;
    }
}

void leaderboardMode(SDL_Renderer *renderer, GameState *gameState)
{
    int startX = SCREEN_WIDTH / 2 - 350;  // Starting X position for the text
    int startY = SCREEN_HEIGHT / 2 - 100; // Starting Y position for the text
    LeaderboardRow leaderboard[10];

    getLeaderboard(leaderboard);

    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color amberColor = {255, 191, 0, 255};

    static int toggle = 0;

    static TTF_Font *mainFont = NULL;
    if (!mainFont)
    {
        mainFont = TTF_OpenFont("fonts/alpha-taurus-font/Main.ttf", 100);
    }

    static TTF_Font *font = NULL;
    if (!font)
    {
        font = TTF_OpenFont("fonts/PS2P/PressStart2P-Regular.ttf", 30);
    }

    int mx;
    getCentreOfText(mainFont, "LEADERBOARD", &mx, NULL);
    renderText(renderer, mainFont, "LEADERBOARD", mx, 100, toggle ? amberColor : whiteColor);
    toggle = !toggle;

    renderText(renderer, font, "> MENU", 100, SCREEN_HEIGHT - 50, toggle ? amberColor : whiteColor);

    for (int i = 0; i < 10; i++)
    {
        // Calculate position for the text
        int x = startX + (i / 5) * 400; // Adjust X for second column
        int y = startY + (i % 5) * 50;  // Y position based on row

        char text[100] = "";
        sprintf(text, "%d. %s %d", i, (leaderboard[i].name[0] != '\0') ? leaderboard[i].name : "___", leaderboard[i].score);
        renderText(renderer, font, text, x, y, whiteColor);
    }

    if (getButtonClick(SDL_SCANCODE_RETURN))
    {
        playSoundEffect('C');
        *gameState = MENU;
    }
}