#ifndef HEAP_H
#define HEAP_H
#include "players.h"


typedef struct {
    PlayerNode *node;
    int teamIndex;
} HeapItem;

typedef struct {
    HeapItem *data;
    int size;
    int cap;
} MaxHeap;


MaxHeap *createHeap(int capacity);
void freeHeap(MaxHeap *currHeap);
void swapHeap(HeapItem *heap1, HeapItem *heap2);
void pushHead(MaxHeap *currHeap, HeapItem item);
HeapItem popHead(MaxHeap *currHeap);


#endif