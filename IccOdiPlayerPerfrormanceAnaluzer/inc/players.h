#ifndef PLAYERS_H
#define PLAYERS_H
#include "Players_data.h"
#include<stddef.h>

#define MAX_TEAMS 10

typedef struct PlayerRecord{
    Player data;
    struct PlayerRecord* next;
    float performanceIndex;
} PlayerRecord;

typedef struct{
    int id;
    char *name;
    PlayerRecord *head;
    PlayerRecord *batsmanHead;
    PlayerRecord *bowlerHead;
    PlayerRecord *allRounderHead;

    int batsmanCount, bowlerCount, allrounderCount;
    int playerCount;
    int avgBattingSr;
} Team;

extern Team *team;
extern Team **teamIndex; 

PlayerRecord *createNodeFromPlayers(const Player *curPlayer);
void insertSortedDesc(PlayerRecord **headPtr, PlayerRecord *node);
float computePerformanceIndex(const Player *curPlayer);
int buildTeamIndex();
int findTeamIndexByName(const char *name);
void initializeTeams();
Player* inputPlayerDetails();
void displayTeamsSortedByAvgStrikeRate();
void displayTeamPlayersById(const int teamId);
int addPlayerToTeamById(const int id, const Player *curPlayer);
void displayTopKofTeamByRole(const char *name, const char *role,const int K);
void displayAllPlayersByRole(const char *role);
void freeAllData();


#endif
