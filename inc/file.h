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

void createFile(char *name);
void writeFile(char *name, char *data);
void readFile(char *name);
void deleteFile(char *name);

void initializeVirtualBlock();
int allocateBlock();
void freeBlock(int index);
int allocateBlocks(int required, int *outArr);
void showDiskDetials();

#endif
