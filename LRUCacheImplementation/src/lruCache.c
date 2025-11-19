#include <stdio.h>
#include <stdlib.h>
#include "lruCache.h"

int hashFunction(const int key, const int hashSize) {
    return key % hashSize;
}

Node* createNode(const int key, const char* value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->prev = node->next = NULL;
    return node;
}

void insertAtHead(LRUCache* cache, Node* node) {
    node->next = cache->head;
    node->prev = NULL;

    if (cache->head){
        cache->head->prev = node;
    }
    cache->head = node;

    if (cache->tail == NULL){
        cache->tail = node;
    }
}

void removeNode(LRUCache* cache, Node* node) {
    if (node->prev){
        node->prev->next = node->next;
    }
    else{
        cache->head = node->next;
    }

    if (node->next){
        node->next->prev = node->prev;
    }
    else{
        cache->tail = node->prev;
    }
}

LRUCache* lruCreate(const int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;

    cache->hashSize = 1000; 
    cache->hash = (Node**)calloc(cache->hashSize, sizeof(Node*));

    cache->head = cache->tail = NULL;
    return cache;
}

char* lruGet(LRUCache* cache, const int key) {
    int index = hashFunction(key, cache->hashSize);
    Node* node = cache->hash[index];

    char* ans;

    if (!node){
        ans = malloc(sizeof(char) * 100); 
        ans = "-1";
    }
    else{
        removeNode(cache, node);
        insertAtHead(cache, node);

        ans = node->value;
    }

    return ans;
}

void lruPut(LRUCache* cache, const int key, const char* value) {
    int index = hashFunction(key, cache->hashSize);
    Node* node = cache->hash[index];

    if (node) {
        node->value = value;

        removeNode(cache, node);
        insertAtHead(cache, node);
    }
    else{
        Node* newNode = createNode(key, value);

        if (cache->size == cache->capacity) {
            int tailKey = cache->tail->key;
            int tailIndex = hashFunction(tailKey, cache->hashSize);
            cache->hash[tailIndex] = NULL;

            removeNode(cache, cache->tail);
            free(cache->tail);
            cache->size--;
        }

        insertAtHead(cache, newNode);
        cache->hash[index] = newNode;
        cache->size++;
    }
}

void lruFree(LRUCache* cache) {
    Node* curr = cache->head;
    while (curr) {
        Node* next = curr->next;
        free(curr);
        curr = next;
    }
    free(cache->hash);
    free(cache);
}
