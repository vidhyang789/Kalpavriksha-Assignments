#include "globals.h"
#include <stddef.h>  


char **virtualDisk;
FreeBlock *freeListHead = NULL;
FreeBlock *freeListTail = NULL;

FileNode *root = NULL;
FileNode *cwd = NULL;
