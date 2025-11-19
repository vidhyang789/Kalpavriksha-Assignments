#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

PlayerMaxHeap *createHeap(const int capacity)
{
    PlayerMaxHeap *currheap = (PlayerMaxHeap *)malloc(sizeof(PlayerMaxHeap));
    if (currheap)
    {
        currheap->data = (HeapItem *)malloc(sizeof(HeapItem) * capacity);
        if (!currheap->data)
        {
            free(currheap);
            currheap = NULL;
        }
        else
        {
            currheap->size = 0;
            currheap->capacity = capacity;
        }
    }
    return currheap;
}

void freeHeap(PlayerMaxHeap *currHeap)
{
    if (currHeap)
    {
        free(currHeap->data);
        free(currHeap);
    }
}

void swapHeap(HeapItem *heap1, HeapItem *heap2)
{
    HeapItem temp = *heap1;
    *heap1 = *heap2;
    *heap2 = temp;
}

void pushHead(PlayerMaxHeap *currHeap, HeapItem item)
{
    if (currHeap->size < currHeap->capacity)
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

HeapItem popHead(PlayerMaxHeap *currHeap)
{
    HeapItem result = currHeap->data[0];
    currHeap->size--;
    currHeap->data[0] = currHeap->data[currHeap->size];
    int index = 0;
    while (1)
    {
        int leftchild = 2 * index + 1, rightchild = 2 * index + 2, largest = index;
        if (leftchild < currHeap->size && currHeap->data[leftchild].node->performanceIndex > currHeap->data[largest].node->performanceIndex)
        {
            largest = leftchild;
        }
        if (rightchild < currHeap->size && currHeap->data[rightchild].node->performanceIndex > currHeap->data[largest].node->performanceIndex)
        {
            largest = rightchild;
        }
        if (largest == index)
        {
            break;
        }
        swapHeap(&currHeap->data[index], &currHeap->data[largest]);
        index = largest;
    }
    return result;
}
