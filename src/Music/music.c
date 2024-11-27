#include "music.h"

void playSoundEffect(char type)
{
    static Mix_Chunk *explosion = NULL;
    static Mix_Chunk *bullet = NULL;
    static Mix_Chunk *menuSwitch = NULL;
    static Mix_Chunk *menuClick = NULL;
    static Mix_Chunk *playerLost = NULL;

    if (explosion == NULL)
        explosion = Mix_LoadWAV("SoundEffects/explosionsound.wav");
    if (bullet == NULL)
        bullet = Mix_LoadWAV("SoundEffects/PlayerShootingSound.wav");
    if (menuSwitch == NULL)
        menuSwitch = Mix_LoadWAV("SoundEffects/Menuswitch.wav");
    if (menuClick == NULL)
        menuClick = Mix_LoadWAV("SoundEffects/Menuclick.wav");
    if (playerLost == NULL)
        playerLost = Mix_LoadWAV("SoundEffects/PlayerlosingSound.wav");

    // Play the appropriate sound effect
    if (type == 'E')
        Mix_PlayChannel(-1, explosion, 0); // -1 chooses the first available channel, 0 means play once
    else if (type == 'B')
        Mix_PlayChannel(-1, bullet, 0);
    else if (type == 'S')
        Mix_PlayChannel(-1, menuSwitch, 0);
    else if (type == 'C')
        Mix_PlayChannel(-1, menuClick, 0);
    else if (type == 'L')
        Mix_PlayChannel(-1, playerLost, 0);
}

void playMusic(char type)
{
    static Mix_Music *gameover = NULL;

    if (gameover == NULL)
        gameover = Mix_LoadMUS("SoundEffects/GameOver.mp3");

    if (type == 'G')
        Mix_PlayMusic(gameover, 1);
}