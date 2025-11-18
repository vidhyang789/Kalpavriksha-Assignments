#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "players.h"
#include "heap.h"

Team *team = NULL;
Team **teamIndex = NULL;

float computePerformanceIndex(const Player *curPlayer)
{
    float performance = 0.0f;
    if (curPlayer)
    {
        if (strcmp(curPlayer->role, "Batsman") == 0)
        {
            performance = (curPlayer->battingAverage * curPlayer->strikeRate) / 100.0f;
        }
        else if (strcmp(curPlayer->role, "Bowler") == 0)
        {
            performance = (curPlayer->wickets * 2.0f) + (100.0f - curPlayer->economyRate);
        }
        else if (strcmp(curPlayer->role, "All-rounder") == 0)
        {
            performance = ((curPlayer->battingAverage * curPlayer->strikeRate) / 100.0f) + (curPlayer->wickets * 2.0f);
        }
    }

    return performance;
}

void insertSortedDesc(PlayerRecord **headPtr, PlayerRecord *node)
{
    if (headPtr && node)
    {
        PlayerRecord *head = *headPtr;
        if (!head || node->performanceIndex >= head->performanceIndex)
        {
            node->next = head;
            *headPtr = node;
        }
        else
        {
            PlayerRecord *cur = head;
            while (cur->next && cur->next->performanceIndex > node->performanceIndex)
            {
                cur = cur->next;
            }
            node->next = cur->next;
            cur->next = node;
        }
    }
}

PlayerRecord *createNodeFromPlayers(const Player *curPlayer)
{
    PlayerRecord *node = NULL;
    node = (PlayerRecord *)malloc(sizeof(PlayerRecord));

    if (node)
    {
        node->data = *curPlayer;
        node->performanceIndex = computePerformanceIndex(curPlayer);
        node->next = NULL;
    }

    return node;
}

int buildTeamIndex()
{
    int result = -1;
    if (team)
    {
        teamIndex = (Team **)malloc(sizeof(Team *) * teamCount);
        if (teamIndex)
        {
            for (int index = 0; index < teamCount; ++index)
            {
                teamIndex[index] = &team[index];
            }

            result = 0;
        }
    }
    return result;
}

int findTeamIndexByName(const char *name)
{
    int idx = -1;
    for(int index = 0;index < teamCount;index++){
        if(strcmp(name,teams[index]) == 0){
            idx = index;
            break;
        }
    }

    return idx;
}

void initializeTeams()
{
    team = malloc(MAX_TEAMS * sizeof(Team));
    if (!team)
    {
        printf("Memory allocation failed for teams\n");
    }
    else
    {
        teamCount = MAX_TEAMS;

        for (int index = 0; index < MAX_TEAMS; index++)
        {
            team[index].id = index + 1;
            team[index].name = malloc(sizeof(char) * 50);
            strcpy(team[index].name, teams[index]);
            team[index].head = NULL;
            team[index].batsmanHead = NULL;
            team[index].bowlerHead = NULL;
            team[index].allRounderHead = NULL;
            team[index].playerCount = 0;
            team[index].batsmanCount = 0;
            team[index].bowlerCount = 0;
            team[index].allrounderCount = 0;
            team[index].avgBattingSr = 0.0f;
        }

        if (buildTeamIndex() != 0)
        {
            printf("Failed to build Team Index\n");
            exit(0);
        }

        for (int index = 0; index < playerCount; ++index)
        {
            const Player *src = &players[index];
            PlayerRecord *node = createNodeFromPlayers(src);
            if (node)
            {
                int teamindex = findTeamIndexByName(src->team);
                if (teamindex < 0)
                {
                    teamindex = 0;
                }

                Team *currTeam = &team[teamindex];

                node->next = currTeam->head;
                currTeam->head = node;

                if (strcmp(src->role, "Batsman") == 0)
                {
                    insertSortedDesc(&currTeam->batsmanHead, node);
                    currTeam->batsmanCount++;
                    currTeam->avgBattingSr += src->strikeRate;
                }
                else if (strcmp(src->role, "Bowler") == 0)
                {
                    insertSortedDesc(&currTeam->bowlerHead, node);
                    currTeam->bowlerCount++;
                }
                else
                {
                    insertSortedDesc(&currTeam->allRounderHead, node);
                    currTeam->allrounderCount++;
                    currTeam->avgBattingSr += src->strikeRate;
                }
                currTeam->playerCount++;
            }
        }

        printf("Initialization complete: loaded %d players into %d teams.\n", playerCount, teamCount);
    }
}

int addPlayerToTeamById(const int id, const Player *curPlayer)
{
    int ans = -1;
    if (team && id >= 1 && id <= teamCount && curPlayer)
    {
        Team *currTeam = &team[id - 1];
        PlayerRecord *node = createNodeFromPlayers(curPlayer);
        if (node)
        {
            node->next = currTeam->head;
            currTeam->head = node;

            if (strcmp(curPlayer->role, "Batsman") == 0)
            {
                insertSortedDesc(&currTeam->batsmanHead, node);
                currTeam->batsmanCount++;
                currTeam->avgBattingSr += curPlayer->strikeRate;
            }
            else if (strcmp(curPlayer->role, "Bowler") == 0)
            {
                insertSortedDesc(&currTeam->bowlerHead, node);
                currTeam->bowlerCount++;
            }
            else
            {
                insertSortedDesc(&currTeam->allRounderHead, node);
                currTeam->allrounderCount++;
                currTeam->avgBattingSr += curPlayer->strikeRate;
            }
            currTeam->playerCount++;
            ans = 0;
        }
    }

    return ans;
}

Player *inputPlayerDetails()
{
    Player *curPlayer = malloc(sizeof(Player));

    printf("Player ID : ");
    scanf("%d", &curPlayer->id);
    printf("Name: ");
    curPlayer->name = malloc(sizeof(char) * 64);
    scanf("%s", curPlayer->name);
    printf("Role (Batsman/Bowler/All-rounder): ");
    curPlayer->role = malloc(sizeof(char) * 32);
    scanf("%s", curPlayer->role);
    printf("TotalRuns: ");
    scanf("%d", &curPlayer->totalRuns);
    printf("BattingAverage: ");
    scanf("%f", &curPlayer->battingAverage);
    printf("StrikeRate: ");
    scanf("%f", &curPlayer->strikeRate);
    printf("Wickets: ");
    scanf("%d", &curPlayer->wickets);
    printf("EconomyRate: ");
    scanf("%f", &curPlayer->economyRate);

    return curPlayer;
}

void displayTeamPlayersById(const int teamId)
{
    if (!team || teamId < 1 || teamId > teamCount)
    {
        printf("Invalid team id\n");
        return;
    }
    else
    {
        Team *currTeam = &team[teamId - 1];

        printf("\nTeam: %s (ID=%d)\n", currTeam->name, currTeam->id);
        printf("Total players = %d\n", currTeam->playerCount);

        int srCount = currTeam->batsmanCount + currTeam->allrounderCount;
        if (srCount > 0)
        {
            printf("Average Batting Strike Rate (Batsmen + All-rounders) = %.2f\n", (float)(currTeam->avgBattingSr / srCount));
        }
        else
        {
            printf("Average Batting Strike Rate: N/A\n");
        }

        printf("\n-- Batsmen (top to bottom) --\n");
        PlayerRecord *cur = currTeam->batsmanHead;
        printf("==================================================================\n");
        printf("ID   Name                          PerfIdx           SR    Runs    \n");
        printf("----+----------------------------+-----------------+-----+--------\n");
        while (cur)
        {
            printf("%-4d|%-28s|%-17.2f|%-5.1f|%-5d\n",
                   cur->data.id, cur->data.name, cur->performanceIndex, cur->data.strikeRate, cur->data.totalRuns);
            cur = cur->next;
        }

        printf("\n-- All-rounders (top to bottom) --\n");
        cur = currTeam->allRounderHead;
        printf("==================================================================\n");
        printf("ID   Name                          PerfIdx           SR    Runs    \n");
        printf("----+----------------------------+-----------------+-----+--------\n");
        while (cur)
        {
            printf("%-4d|%-28s|%-17.2f|%-5.1f|%-5d\n",
                   cur->data.id, cur->data.name, cur->performanceIndex, cur->data.strikeRate, cur->data.totalRuns);
            cur = cur->next;
        }

        printf("\n-- Bowlers (top to bottom) --\n");
        cur = currTeam->bowlerHead;
        printf("==================================================================\n");
        printf("ID   Name                          PerfIdx           SR    Runs    \n");
        printf("----+----------------------------+-----------------+-----+--------\n");
        while (cur)
        {
            printf("%-4d|%-28s|%-17.2f|%-5.1f|%-5d\n",
                   cur->data.id, cur->data.name, cur->performanceIndex, cur->data.strikeRate, cur->data.totalRuns);
            cur = cur->next;
        }
    }
}

int compareTeamsByAvgSR(const void *a, const void *b)
{
    Team *A = *(Team **)a;
    Team *B = *(Team **)b;

    int ac = A->batsmanCount + A->allrounderCount;
    int bc = B->batsmanCount + B->allrounderCount;

    float avA = (ac > 0) ? (A->avgBattingSr / ac) : 0.0f;
    float avB = (bc > 0) ? (B->avgBattingSr / bc) : 0.0f;

    if (avA < avB) return 1;   // sort descending
    if (avA > avB) return -1;
    return 0;
}

void displayTeamsSortedByAvgStrikeRate()
{
    if (team)
    {
        Team **arr = (Team **)malloc(sizeof(Team *) * teamCount);
        if (arr)
        {
            for (int index = 0; index < teamCount; ++index)
            {
                arr[index] = &team[index];
            }

            qsort(arr, teamCount, sizeof(Team *), compareTeamsByAvgSR);

            printf("\nTeams sorted by Average Batting Strike Rate (desc):\n");
            printf("================================================\n");
            printf("Rank | Team                 | AvgSR   | #Players\n");
            printf("-----+----------------------+---------+---------\n");
            for (int index = 0; index < teamCount; ++index)
            {
                Team *currTeam = arr[index];
                int c = currTeam->batsmanCount + currTeam->allrounderCount;
                float av = (c > 0) ? (currTeam->avgBattingSr / c) : 0.0f;
                printf("%4d  %-20s  %7.2f   %6d\n", index + 1, currTeam->name, av, currTeam->playerCount);
            }

            free(arr);
        }
    }
}

void displayTopKofTeamByRole(const int index, const char *role,const int K)
{
    if (!team || !role || K <= 0)
    {
        printf("Invalid input\n");
    }
    else
    {
        if (index < 0 || index >= teamCount)
        {
            printf("Team '%d' is out of bound\n", index);
        }
        else
        {
            Team* currTeam = teamIndex[index];
            PlayerRecord *cur = NULL;

            if (strcmp(role, "Batsman") == 0)
            {
                cur = currTeam->batsmanHead;
            }
            else if (strcmp(role, "Bowler") == 0)
            {
                cur = currTeam->bowlerHead;
            }
            else
            {
                cur = currTeam->allRounderHead;
            }

            if (!cur)
            {
                printf("No players with role '%s' in team '%s'\n", role, currTeam->name);
            }
            else
            {
                printf("\nTop %d %s of %s (by Performance Index):\n", K, role, currTeam->name);
                printf("===============================================================================================\n");
                printf("Id  | PlayerName                | PerformanceIndex    | Runs       | Wickets    | SR     | ER  \n");
                printf("----+---------------------------+---------------------+------------+------------+--------+-----\n");
                int printed = 0;
                while (cur && printed < K)
                {
                    printf("%-3d | %-25s | %-19.2f | %-10d | %-10d | %-6.1f | %.1f\n",
                           cur->data.id, cur->data.name, cur->performanceIndex,
                           cur->data.totalRuns, cur->data.wickets,
                           cur->data.strikeRate, cur->data.economyRate);
                    cur = cur->next;
                    printed++;
                }
            }
        }
    }
}

void displayAllPlayersByRole(const char *role)
{
    if (role && team)
    {
        PlayerMaxHeap *heap = createHeap(teamCount);
        if (heap)
        {
            for (int index = 0; index < teamCount; index++)
            {
                PlayerRecord *start = NULL;
                if (strcmp(role, "Batsman") == 0)
                {
                    start = team[index].batsmanHead;
                }
                else if (strcmp(role, "Bowler") == 0)
                {
                    start = team[index].bowlerHead;
                }
                else
                {
                    start = team[index].allRounderHead;
                }

                if (start)
                {
                    HeapItem node;
                    node.node = start;
                    node.teamIndex = index;
                    pushHead(heap, node);
                }
            }

            if (heap->size == 0)
            {
                printf("No players found for role '%s'\n", role);
                freeHeap(heap);
            }
            else
            {
                printf("\nAll players for role '%s' sorted by Performance Index (desc):\n", role);
                printf("=======================================================================================================\n");
                printf("ID   | Name                      | Team            | Role       | PI    | Runs  | Wkts  | SR    | ER\n");
                printf("-----+---------------------------+-----------------+------------+-------+-------+-------+-------+------\n");
                int count = 0;
                while (heap->size > 0)
                {
                    HeapItem top = popHead(heap);
                    PlayerRecord *node = top.node;
                    int teamindex = top.teamIndex;
                    printf("%-4d | %-25s | %-15s | %-10s | %-5.2f | %-5d | %-5d | %-5.1f | %-5.1f\n",
                           node->data.id, node->data.name, team[teamindex].name, node->data.role,
                           node->performanceIndex, node->data.totalRuns, node->data.wickets, node->data.strikeRate, node->data.economyRate);

                    PlayerRecord *next = NULL;
                    next = node->next;

                    if (next)
                    {
                        HeapItem nextTeamindex;
                        nextTeamindex.node = next;
                        nextTeamindex.teamIndex = teamindex;
                        pushHead(heap, nextTeamindex);
                    }
                    count++;
                }
                printf("\nTotal %d players are there \n",count);
            }
        }
    }
}

void freeAllData()
{
    if (team)
    {
        if (teamIndex)
        {
            free(teamIndex);
            teamIndex = NULL;
        }
        if (team)
        {
            free(team);
            team = NULL;
        }
        teamCount = 0;
    }
}
