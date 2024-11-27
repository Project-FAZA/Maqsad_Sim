#include "music.h"

void playSoundEffect(char type)
{
    Mix_VolumeMusic(128);
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
        Mix_PlayChannel(0, explosion, 0);
    else if (type == 'B')
        Mix_PlayChannel(1, bullet, 0);
    else if (type == 'S')
        Mix_PlayChannel(2, menuSwitch, 0);
    else if (type == 'C')
        Mix_PlayChannel(3, menuClick, 0);
    else if (type == 'L')
        Mix_PlayChannel(4, playerLost, 0);
}

void playMusic(char type)
{
    Mix_VolumeMusic(128);
    static Mix_Music *battle = NULL;
    static Mix_Music *gameover = NULL;

    if (battle == NULL)
    {
        Mix_VolumeMusic(64);
        battle = Mix_LoadMUS("SoundEffects/Battle.mp3");
    }

    if (gameover == NULL)
        gameover = Mix_LoadMUS("SoundEffects/GameOver.mp3");

    if (type == 'B')
        Mix_PlayMusic(battle, -1);

    else if (type == 'G')
        Mix_PlayMusic(gameover, 1);
}
