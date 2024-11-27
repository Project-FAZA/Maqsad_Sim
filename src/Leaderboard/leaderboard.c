#include "leaderboard.h"

void getLeaderboard(LeaderboardRow leaderboard[10])
{
    FILE *file = fopen("leaderboard.dat", "r+b");

    if (file == NULL)
    {
        file = fopen("leaderboard.dat", "w+b");

        if (file == NULL)
        {
            printf("Err creating file");
            return;
        }

        for (int i = 0; i < 10; i++)
        {
            strcpy(leaderboard[i].name, "");
            leaderboard[i].score = 0;
        }

        fwrite(leaderboard, sizeof(LeaderboardRow), 10, file);
    }
    else
    {
        fread(leaderboard, sizeof(LeaderboardRow), 10, file);
    }

    fclose(file);
}

void sortLeaderboard(LeaderboardRow leaderboard[10])
{
    for (int i = 0; i < 10 - 1; i++)
    {
        for (int j = 0; j < 10 - i - 1; j++)
        {
            if (leaderboard[j].score < leaderboard[j + 1].score)
            {
                // Swap the rows
                LeaderboardRow temp = leaderboard[j];
                leaderboard[j] = leaderboard[j + 1];
                leaderboard[j + 1] = temp;
            }
        }
    }
}

void saveLeaderboard(LeaderboardRow leaderboard[10])
{
    sortLeaderboard(leaderboard);

    FILE *file = fopen("leaderboard.dat", "wb");

    fwrite(leaderboard, sizeof(LeaderboardRow), 10, file);

    fclose(file);
}

int getLeaderboardCount(LeaderboardRow leaderboard[10])
{
    int count = 0;

    for (int i = 0; i < 10; i++)
    {
        if (leaderboard[i].name[0] == '\0')
            break;

        count++;
    }

    return count;
}

int getMinIndex(LeaderboardRow leaderboard[10])
{
    int minIndex = 0;
    int count = getLeaderboardCount(leaderboard);

    if (count == 0)
        return -1;

    for (int i = 1; i < 10; i++)
    {
        if (leaderboard[i].score < leaderboard[minIndex].score)
            minIndex = i;
    }

    return minIndex;
}

/*
    0 -> No update
    1 -> Highscore updated
    2 -> 1st Position achieved
*/
int updateLeaderboard(char name[4], int score)
{
    LeaderboardRow leaderboard[10];

    getLeaderboard(leaderboard);
    int count = getLeaderboardCount(leaderboard);

    int minIndex = getMinIndex(leaderboard);

    if ((minIndex != -1) && (score < leaderboard[minIndex].score))
        return 0;

    if (count == 0)
    {
        strcpy(leaderboard[0].name, name);
        leaderboard[0].score = score;
        saveLeaderboard(leaderboard);
        return 2;
    }

    for (int i = 0; i < 10; i++)
    {
        if (strcmp(name, leaderboard[i].name) == 0)
        {
            if (score < leaderboard[i].score)
                return 0;

            leaderboard[i].score = score;
            saveLeaderboard(leaderboard);

            if (i == 0)
                return 2;
            else
                return 1;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        // if the score manages to enter the leaderboard, phir sab se last position wala shaat hojayega
        if (score > leaderboard[i].score)
        {
            strcpy(leaderboard[minIndex].name, name);
            leaderboard[minIndex].score = score;

            saveLeaderboard(leaderboard);

            if (i == 0)
                return 2;
            else
                return 1;
        }
    }

    return 0;
}

void displayLeaderboard()
{
    LeaderboardRow leaderboard[10];

    getLeaderboard(leaderboard);

    int count = getLeaderboardCount(leaderboard);
    printf("%d", count);
    for (int i = 0; i < 10; i++)
    {
        printf("\n%d) %s %d", i + 1, leaderboard[i].name, leaderboard[i].score);
    }
}