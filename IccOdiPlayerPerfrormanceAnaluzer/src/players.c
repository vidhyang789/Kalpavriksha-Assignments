#include "players.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Team *team = NULL;
Team **teamIndex = NULL;

float computePerformanceIndex(const Player *p)
{
    float ans = 0.0f;
    if (p)
    {
        if (strcmp(p->role, "Batsman") == 0)
        {
            ans = (p->battingAverage * p->strikeRate) / 100.0f;
        }
        else if (strcmp(p->role, "Bowler") == 0)
        {
            ans = (p->wickets * 2.0f) + (100.0f - p->economyRate);
        }
        else if (strcmp(p->role, "All-rounder") == 0)
        {
            ans = ((p->battingAverage * p->strikeRate) / 100.0f) + (p->wickets * 2.0f);
        }
    }

    return ans;
}

void insertSortedDesc(PlayerNode **headPtr, PlayerNode *node)
{
    if (headPtr && node)
    {
        PlayerNode *head = *headPtr;
        if (!head || node->performanceIndex >= head->performanceIndex)
        {
            node->next = head;
            *headPtr = node;
        }
        else
        {
            PlayerNode *cur = head;
            while (cur->next && cur->next->performanceIndex > node->performanceIndex)
            {
                cur = cur->next;
            }
            node->next = cur->next;
            cur->next = node;
        }
    }
}

PlayerNode *createNodeFromPlayers(const Player *p)
{
    PlayerNode *n = (PlayerNode *)malloc(sizeof(PlayerNode));

    if (n)
    {
        n->data = *p;
        n->performanceIndex = computePerformanceIndex(p);
        n->next = NULL;
    }

    return (n ? n : NULL);
}

int buildTeamIndex()
{
    int ans = -1;
    if (team)
    {
        teamIndex = (Team **)malloc(sizeof(Team *) * teamCount);
        if (teamIndex)
        {
            for (int i = 0; i < teamCount; ++i)
            {
                teamIndex[i] = &team[i];
            }

            int cmp(const void *a, const void *b)
            {
                Team *A = *(Team **)a;
                Team *B = *(Team **)b;
                return strcasecmp(A->name, B->name);
            }

            qsort(teamIndex, teamCount, sizeof(Team *), cmp);
            ans = 0;
        }
    }
    return ans;
}

int findTeamIndexByName(const char *name)
{
    int ans = -1;

    if (teamIndex && name)
    {
        int l = 0, r = teamCount - 1;
        while (l <= r)
        {
            int mid = (l + r) / 2;
            int cmp = strcasecmp(teamIndex[mid]->name, name);
            if (cmp == 0)
            {
                ans = (teamIndex[mid] - team);
                break;
            }
            else if (cmp < 0)
            {
                l = mid + 1;
            }
            else
            {
                r = mid - 1;
            }
        }
    }

    return ans;
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

        for (int i = 0; i < MAX_TEAMS; i++)
        {
            team[i].id = i + 1;
            team[i].name = malloc(sizeof(char) * 50);
            strcpy(team[i].name, teams[i]);
            team[i].head = NULL;
            team[i].batsmanHead = NULL;
            team[i].bowlerHead = NULL;
            team[i].allRounderHead = NULL;
            team[i].playerCount = 0;
            team[i].batsmanCount = 0;
            team[i].bowlerCount = 0;
            team[i].allrounderCount = 0;
            team[i].avgBattingSr = 0.0f;
        }

        if (buildTeamIndex() != 0)
        {
            printf("Failed to build Team Index\n");
            exit(0);
        }

        for (int i = 0; i < playerCount; ++i)
        {
            const Player *src = &players[i];
            PlayerNode *node = createNodeFromPlayers(src);
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

int addPlayerToTeamById(int id, const Player *p)
{
    int ans = -1;
    if (team && id >= 1 && id <= teamCount && p)
    {
        Team *currTeam = &team[id - 1];
        PlayerNode *node = createNodeFromPlayers(p);
        if (node)
        {
            node->next = currTeam->head;
            currTeam->head = node;

            if (strcmp(p->role, "Batsman") == 0)
            {
                insertSortedDesc(&currTeam->batsmanHead, node);
                currTeam->batsmanCount++;
                currTeam->avgBattingSr += p->strikeRate;
            }
            else if (strcmp(p->role, "Bowler") == 0)
            {
                insertSortedDesc(&currTeam->bowlerHead, node);
                currTeam->bowlerCount++;
            }
            else
            {
                insertSortedDesc(&currTeam->allRounderHead, node);
                currTeam->allrounderCount++;
                currTeam->avgBattingSr += p->strikeRate;
            }
            currTeam->playerCount++;
            ans = 0;
        }
    }

    return ans;
}

Player *inputPlayerDetails()
{
    Player *p = malloc(sizeof(Player));

    printf("Player ID : ");
    scanf("%d", &p->id);
    printf("Name: ");
    p->name = malloc(sizeof(char) * 64);
    scanf("%s", p->name);
    printf("Role (Batsman/Bowler/All-rounder): ");
    p->role = malloc(sizeof(char) * 32);
    scanf("%s", p->role);
    printf("TotalRuns: ");
    scanf("%d", &p->totalRuns);
    printf("BattingAverage: ");
    scanf("%f", &p->battingAverage);
    printf("StrikeRate: ");
    scanf("%f", &p->strikeRate);
    printf("Wickets: ");
    scanf("%d", &p->wickets);
    printf("EconomyRate: ");
    scanf("%f", &p->economyRate);

    return p;
}

void displayTeamPlayersById(int teamId)
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
        PlayerNode *cur = currTeam->batsmanHead;
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

void displayTeamsSortedByAvgStrikeRate()
{
    if (team)
    {
        Team **arr = (Team **)malloc(sizeof(Team *) * teamCount);
        if (arr)
        {
            for (int i = 0; i < teamCount; ++i)
            {
                arr[i] = &team[i];
            }

            int cmp(const void *a, const void *b)
            {
                int ans = 0;
                Team *A = *(Team **)a;
                Team *B = *(Team **)b;
                int ac = A->batsmanCount + A->allrounderCount;
                int bc = B->batsmanCount + B->allrounderCount;
                float avA = (ac > 0) ? (A->avgBattingSr / ac) : 0.0f;
                float avB = (bc > 0) ? (B->avgBattingSr / bc) : 0.0f;
                if (avA < avB)
                    ans = 1;
                if (avA > avB)
                    ans = -1;
                return ans;
            }

            qsort(arr, teamCount, sizeof(Team *), cmp);

            printf("\nTeams sorted by Average Batting Strike Rate (desc):\n");
            printf("================================================\n");
            printf("Rank | Team                 | AvgSR   | #Players\n");
            printf("-----+----------------------+---------+---------\n");
            for (int i = 0; i < teamCount; ++i)
            {
                Team *currTeam = arr[i];
                int c = currTeam->batsmanCount + currTeam->allrounderCount;
                float av = (c > 0) ? (currTeam->avgBattingSr / c) : 0.0f;
                printf("%4d  %-20s  %7.2f   %6d\n", i + 1, currTeam->name, av, currTeam->playerCount);
            }

            free(arr);
        }
    }
}

void displayTopKofTeamByRole(const char *name, const char *role, int K)
{
    if (!team || !name || !role || K <= 0)
    {
        printf("Invalid input\n");
    }
    else
    {
        int idx = findTeamIndexByName(name);

        if (idx < 0)
        {
            printf("Team '%s' not found\n", name);
        }
        else
        {
            Team *currTeam = &team[idx];
            PlayerNode *cur = NULL;

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
        MaxHeap *heap = createHeap(teamCount);
        if (heap)
        {
            for (int i = 0; i < teamCount; i++)
            {
                PlayerNode *start = NULL;
                if (strcmp(role, "Batsman") == 0)
                {
                    start = team[i].batsmanHead;
                }
                else if (strcmp(role, "Bowler") == 0)
                {
                    start = team[i].bowlerHead;
                }
                else
                {
                    start = team[i].allRounderHead;
                }

                if (start)
                {
                    HeapItem node;
                    node.node = start;
                    node.teamIndex = i;
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

                while (heap->size > 0)
                {
                    HeapItem top = popHead(heap);
                    PlayerNode *node = top.node;
                    int teamindex = top.teamIndex;
                    printf("%-4d | %-25s | %-15s | %-10s | %-5.2f | %-5d | %-5d | %-5.1f | %-5.1f\n",
                           node->data.id, node->data.name, team[teamindex].name, node->data.role,
                           node->performanceIndex, node->data.totalRuns, node->data.wickets, node->data.strikeRate, node->data.economyRate);

                    PlayerNode *next = NULL;
                    next = node->next;

                    if (next)
                    {
                        HeapItem nextTeamindex;
                        nextTeamindex.node = next;
                        nextTeamindex.teamIndex = teamindex;
                        pushHead(heap, nextTeamindex);
                    }
                }
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
