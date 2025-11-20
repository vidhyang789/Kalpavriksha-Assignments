#ifndef HEAP_H
#define HEAP_H
#include "players.h"

typedef struct
{
    PlayerRecord *node;
    int teamIndex;
} HeapItem;

typedef struct
{
    HeapItem *data;
    int size;
    int capacity;
} PlayerMaxHeap;

PlayerMaxHeap *createHeap(int capacity);
void freeHeap(PlayerMaxHeap *currHeap);
void swapHeap(HeapItem *heap1, HeapItem *heap2);
void pushHead(PlayerMaxHeap *currHeap, HeapItem item);
HeapItem popHead(PlayerMaxHeap *currHeap);

#endif
