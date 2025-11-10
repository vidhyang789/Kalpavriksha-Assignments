#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "directory.h"

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

extern char **virtualDisk;
extern FreeBlock *freeListHead, *freeListTail;

void createCmd(char *name);
void writeCmd(char *name, char *data);
void readCmd(char *name);
void deleteCmd(char *name);

void initializeVirtualBlock();
int allocateBlock();
void freeBlock(int index);
int allocateBlocks(int required, int *outArr);
void dfCmd();

#endif
