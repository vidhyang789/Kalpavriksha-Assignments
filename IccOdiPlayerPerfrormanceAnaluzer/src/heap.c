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
        int index = currHeap->size++;
        currHeap->data[index] = item;
        while (index > 0)
        {
            int parent = (index - 1) / 2;
            if (currHeap->data[parent].node->performanceIndex >= currHeap->data[index].node->performanceIndex)
            {
                break;
            }
            swapHeap(&currHeap->data[parent], &currHeap->data[index]);
            index = parent;
        }
    }
}

HeapItem popHead(MaxHeap *currHeap)
{
    HeapItem res = currHeap->data[0];
    currHeap->size--;
    currHeap->data[0] = currHeap->data[currHeap->size];
    int index = 0;
    while (1)
    {
        int l = 2 * index + 1, r = 2 * index + 2, largest = index;
        if (l < currHeap->size && currHeap->data[l].node->performanceIndex > currHeap->data[largest].node->performanceIndex)
        {
            largest = l;
        }
        if (r < currHeap->size && currHeap->data[r].node->performanceIndex > currHeap->data[largest].node->performanceIndex)
        {
            largest = r;
        }
        if (largest == index)
        {
            break;
        }
        swapHeap(&currHeap->data[index], &currHeap->data[largest]);
        index = largest;
    }
    return res;
}
