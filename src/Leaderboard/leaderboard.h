#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdio.h>
#include <string.h>

typedef struct
{
    char name[4];
    int score;
} LeaderboardRow;

void getLeaderboard(LeaderboardRow leaderboard[10]);
void sortLeaderboard(LeaderboardRow leaderboard[10]);
void saveLeaderboard(LeaderboardRow leaderboard[10]);
int getLeaderboardCount(LeaderboardRow leaderboard[10]);
int getMinIndex(LeaderboardRow leaderboard[10]);
int updateLeaderboard(char name[4], int score);
void displayLeaderboard();

#endif
