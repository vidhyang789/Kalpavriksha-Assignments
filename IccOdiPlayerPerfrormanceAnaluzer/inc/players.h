#ifndef PLAYERS_H
#define PLAYERS_H
#include "Players_data.h"
#include<stddef.h>

#define MAX_TEAMS 10

typedef struct PlayerNode{
    Player data;
    struct PlayerNode* next;
    float performanceIndex;
}PlayerNode;

typedef struct{
    int id;
    char *name;
    PlayerNode *head;
    PlayerNode *batsmanHead;
    PlayerNode *bowlerHead;
    PlayerNode *allRounderHead;

    int batsmanCount, bowlerCount, allrounderCount;
    int playerCount;
    int avgBattingSr;
}Team;

extern Team *team;
extern Team **teamIndex; 

PlayerNode *createNodeFromPlayers(const Player *p);
void insertSortedDesc(PlayerNode **headPtr, PlayerNode *node);
float computePerformanceIndex(const Player *p);
int buildTeamIndex();
int findTeamIndexByName(const char *name);
void initializeTeams();
Player* inputPlayerDetails();
void displayTeamsSortedByAvgStrikeRate();
void displayTeamPlayersById(int teamId);
int addPlayerToTeamById(int id, const Player *p);
void displayTopKofTeamByRole(const char *name, const char *role, int K);
void displayAllPlayersByRole(const char *role);
void freeAllData();


#endif