#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

extern char **virtualDisk;
extern FreeBlock *freeListHead;
extern FreeBlock *freeListTail;

extern FileNode *root;
extern FileNode *cwd;

typedef enum CommandType
{
    MKDIR,
    RMDIR,
    CD,
    PWD,
    CREATE,
    WRITE,
    READ,
    DELETE,
    LS,
    DF,
    EXIT,
    INVALID
} CommandType;

CommandType getCommandType(const char *input);
void showPrompt();
