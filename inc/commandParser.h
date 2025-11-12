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
    CMD_MKDIR,
    CMD_RMDIR,
    CMD_CD,
    CMD_PWD,
    CMD_CREATE,
    CMD_WRITE,
    CMD_READ,
    CMD_DELETE,
    CMD_LS,
    CMD_DF,
    CMD_EXIT,
    CMD_INVALID
} CommandType;

CommandType getCommandType(char *input);
void showPrompt();
