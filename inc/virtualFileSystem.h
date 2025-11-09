#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

typedef enum
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

extern char **virtualDisk;
extern FreeBlock *freeListHead, *freeListTail;
extern FileNode *root, *cwd;


void initializeRootDirectory();
void initializeVirtualBlock();
void initializeDisk();
bool checkIfAlreadyExist(char *name);
bool initializeFileNode(char *name, int isDirectory);
void mkdirCmd(char *name);
void createCmd(char *name);
void lsCmd();
int allocateBlock();
void freeBlock(int index);
int allocateBlocks(int required, int *outArr);
void writeCmd(char *name, char *data);
void readCmd(char *name);
void deleteCmd(char *name);
void rmdirCmd(char *name);
void pwdCmd();
void cdCmd(char *name);
void dfCmd();
void freeFileTree(FileNode *node);
void exitCmd();