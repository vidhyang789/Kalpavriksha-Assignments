#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "players.h"

void readLine(char *buf, int size)
{
    if (!fgets(buf, size, stdin))
    {
        buf[0] = '\0';
    }
    else
    {
        int length = strlen(buf);
        if (length && buf[length - 1] == '\n')
        {
            buf[length - 1] = '\0';
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

    while (1)
    {
        printf("\nEnter choice: ");
        int ch = 0;
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            printf("Enter Team ID to add player: ");
            int tid;
            scanf("%d", &tid);
            printf("Enter Player details : \n");
            Player *p = inputPlayerDetails();
            if (addPlayerToTeamById(tid, p) == 0)
            {
                printf("\nPlayer added successfully to team %s\n", teams[tid]);
            }
            else
            {
                printf("\nPlayer Not added\n");
            }
            break;

        case 2:
            int id;
            printf("Enter Team ID to add player ");
            scanf("%d", &id);
            displayTeamPlayersById(id);
            break;

        case 3:
            displayTeamsSortedByAvgStrikeRate();
            break;

        case 4:
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

        case 5:
            char *r = malloc(sizeof(char) * 30);
            printf("Enter Role (1.Batsman/2.Bowler/3.All-rounder): ");
            scanf("%s", r);
            displayAllPlayersByRole(r);
            break;

        case 6:
            exit(0);

        default:
            printf("Invalid choice\n");
            break;
        }
    }

    freeAllData();
    return 0;
}
