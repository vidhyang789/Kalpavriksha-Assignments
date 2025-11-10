#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct FreeBlock
{
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef struct FileNode
{
    char *name;
    int isDirectory;

    struct FileNode *parent;
    struct FileNode *next;
    struct FileNode *prev;

    struct FileNode *child;

    int size;
    int blockCount;
    int *blockPointers;
} FileNode;

extern FileNode *root, *cwd;

void initializeDisk();
void initializeRootDirectory();
bool checkIfAlreadyExist(char *name);
bool initializeFileNode(char *name, int isDirectory);

void mkdirCmd(char *name);
void rmdirCmd(char *name);
void lsCmd();
void cdCmd(char *name);
void pwdCmd();

void freeFileTree(FileNode *node);
void exitCmd();

#endif
