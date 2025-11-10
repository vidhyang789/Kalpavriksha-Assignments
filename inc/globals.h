#ifndef GLOBALS_H
#define GLOBALS_H

#include <stddef.h>   
#include "file.h"      


extern char **virtualDisk;
extern FreeBlock *freeListHead;
extern FreeBlock *freeListTail;

extern FileNode *root;
extern FileNode *cwd;

#endif 
