#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "players.h"

typedef enum CommandType
{
    AddPlayerToTeam = 1,
    DisplayPlayersOfASpecificTeam,
    DisplayByAverageBattingStrike,
    DisplayTopKPlayersOfTeam,
    DisplayAllPlayersOfSpecificRole,
    Exit
} CommandType;

void readLine(char *buffer, const int size)
{
    if (!fgets(buffer, size, stdin))
    {
        buffer[0] = '\0';
    }
    else
    {
        int length = strlen(buffer);
        if (length && buffer[length - 1] == '\n')
        {
            buffer[length - 1] = '\0';
        }
    }
}

int main(void)
{
    initializeTeams();

    printf("==============================================================================\n");
    printf("                     ICC ODI Player Performance Analyzer                      \n");
    printf("==============================================================================\n");
    printf("1. Add Player to Team\n");
    printf("2. Display Players of a Specific Team\n");
    printf("3. Display Teams by Average Batting Strike Rate\n");
    printf("4. Display Top K Players of a Specific Team by Role\n");
    printf("5. Display all Players of specific role Across All Teams by performance index\n");
    printf("6. Exit\n");

    int running = 1;
    while (running)
    {
        printf("\nEnter choice: ");
        int ch = 0;
        scanf("%d", &ch);

        switch (ch)
        {
        case AddPlayerToTeam:
            printf("Enter Team ID to add player: ");
            int teamId;
            scanf("%d", &teamId);
            printf("Enter Player details : \n");
            Player *player = inputPlayerDetails();
            if (addPlayerToTeamById(teamId, player) == 0)
            {
                printf("\nPlayer added successfully to team %s\n", teams[teamId]);
            }
            else
            {
                printf("\nPlayer Not added\n");
            }
            break;

        case DisplayPlayersOfASpecificTeam:
            int id;
            printf("Enter Team ID to add player ");
            scanf("%d", &id);
            displayTeamPlayersById(id);
            break;

        case DisplayByAverageBattingStrike:
            displayTeamsSortedByAvgStrikeRate();
            break;

        case DisplayTopKPlayersOfTeam:
            char *tname = malloc(sizeof(char) * 30), *role = malloc(sizeof(char) * 30);
            int K;
            printf("Enter Team name: ");
            scanf("%s", tname);
            printf("Enter Role (Batsman/Bowler/All-rounder): ");
            scanf("%s", role);
            printf("Enter K: ");
            scanf("%d", &K);
            displayTopKofTeamByRole(tname, role, K);
            break;

        case DisplayAllPlayersOfSpecificRole:
            char *r = malloc(sizeof(char) * 30);
            printf("Enter Role (1.Batsman/2.Bowler/3.All-rounder): ");
            scanf("%s", r);
            displayAllPlayersByRole(r);
            break;

        case Exit:
            running = 0;
            break;

        default:
            printf("Invalid choice\n");
            break;
        }
    }

    freeAllData();
    return 0;
}
