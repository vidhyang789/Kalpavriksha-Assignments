#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node{
    char* value;
    int key;
    struct Node* next;
    struct Node* prev;
}Node;

typedef struct LRUCache{
    int capacity;
    int size;
    Node **hash;   
    int hashSize;     
    Node *head;        
    Node *tail;      
} LRUCache;

int hashFunction(const int key, const int hashSize);
LRUCache* lruCreate(const int capacity);
void insertAtHead(LRUCache* cache, Node* node);
Node* createNode(const int key, const char* value);
void removeNode(LRUCache* cache, Node* node);
char* lruGet(LRUCache* cache, const int key);
void lruPut(LRUCache* cache, const int key, const char* value);
void lruFree(LRUCache* cache);

#endif