#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

MaxHeap *createHeap(int capacity)
{
    MaxHeap *currheap = (MaxHeap *)malloc(sizeof(MaxHeap));
    if (currheap)
    {
        currheap->data = (HeapItem *)malloc(sizeof(HeapItem) * capacity);
        if (!currheap->data)
        {
            free(currheap);
        }
        else
        {
            currheap->size = 0;
            currheap->cap = capacity;
        }
    }
    return (currheap ? currheap : NULL);
}

void freeHeap(MaxHeap *currHeap)
{
    if (currHeap)
    {
        free(currHeap->data);
        free(currHeap);
    }
}

void swapHeap(HeapItem *heap1, HeapItem *heap2)
{
    HeapItem tmp = *heap1;
    *heap1 = *heap2;
    *heap2 = tmp;
}

void pushHead(MaxHeap *currHeap, HeapItem item)
{
    if (currHeap->size < currHeap->cap)
    {
        int i = currHeap->size++;
        currHeap->data[i] = item;
        while (i > 0)
        {
            int parent = (i - 1) / 2;
            if (currHeap->data[parent].node->performanceIndex >= currHeap->data[i].node->performanceIndex)
            {
                break;
            }
            swapHeap(&currHeap->data[parent], &currHeap->data[i]);
            i = parent;
        }
    }
}

HeapItem popHead(MaxHeap *currHeap)
{
    HeapItem res = currHeap->data[0];
    currHeap->size--;
    currHeap->data[0] = currHeap->data[currHeap->size];
    int i = 0;
    while (1)
    {
        int l = 2 * i + 1, r = 2 * i + 2, largest = i;
        if (l < currHeap->size && currHeap->data[l].node->performanceIndex > currHeap->data[largest].node->performanceIndex)
        {
            largest = l;
        }
        if (r < currHeap->size && currHeap->data[r].node->performanceIndex > currHeap->data[largest].node->performanceIndex)
        {
            largest = r;
        }
        if (largest == i)
        {
            break;
        }
        swapHeap(&currHeap->data[i], &currHeap->data[largest]);
        i = largest;
    }
    return res;
}
