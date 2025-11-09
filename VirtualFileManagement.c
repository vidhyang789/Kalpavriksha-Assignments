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

char **virtualDisk;
FreeBlock *freeListHead = NULL, *freeListTail = NULL;
FileNode *root = NULL, *cwd = NULL;

void initializeRootDirectory()
{
    root = (FileNode *)malloc(sizeof(FileNode));
    if (!root)
    {
        printf("Memory allocation failed for root directory.\n");
        exit(1);
    }

    root->name = (char *)malloc(50 * sizeof(char));
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->parent = NULL;
    root->next = NULL;
    root->prev = NULL;
    root->child = NULL;
    root->size = 0;
    root->blockCount = 0;
    root->blockPointers = NULL;

    cwd = root;
    printf("   Root directory created: '/'\n\n");
}

void initializeVirtualBlock()
{
    virtualDisk = (char **)malloc(NUM_BLOCKS * sizeof(char *));
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        virtualDisk[i] = (char *)calloc(BLOCK_SIZE, sizeof(char));
    }
}

void initializeDisk()
{

    initializeVirtualBlock();

    for (int index = 0; index < NUM_BLOCKS; index++)
    {
        FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
        if (!newBlock)
        {
            printf("Memory allocation failed during disk initialization.\n");
            exit(1);
        }

        newBlock->index = index;
        newBlock->next = NULL;
        newBlock->prev = freeListTail;
        if (!freeListHead)
        {
            freeListHead = newBlock;
        }
        else
        {
            freeListTail->next = newBlock;
        }
        freeListTail = newBlock;
    }
    initializeRootDirectory();
    printf("$ ./vfs\n");
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}

bool checkIfAlreadyExist(char *name)
{
    FileNode *temp = cwd->child;
    bool isSame = false;
    if (temp)
    {
        do
        {
            if (strcmp(temp->name, name) == 0)
            {
                printf("Directory or File %s already exist\n", name);
                isSame = true;
                break;
            }
            temp = temp->next;
        } while (temp != cwd->child);
    }
    return isSame;
}

bool initializeFileNode(char *name, int isDirectory)
{
    bool ans = false;
    if (!checkIfAlreadyExist(name))
    {
        FileNode *newDir = malloc(sizeof(FileNode));
        newDir->name = malloc(50 * sizeof(char));
        strcpy(newDir->name, name);
        newDir->isDirectory = isDirectory;
        newDir->parent = cwd;
        newDir->child = NULL;
        newDir->blockCount = 0;
        newDir->blockPointers = NULL;
        newDir->size = 0;

        if (!cwd->child)
        {
            cwd->child = newDir;
            newDir->next = newDir->prev = newDir;
        }
        else
        {
            FileNode *head = cwd->child;
            FileNode *tail = head->prev;

            tail->next = newDir;
            newDir->next = head;
            newDir->prev = tail;
            head->prev = newDir;
        }
        ans = true;
    }
    return ans;
}

void mkdirCmd(char *name)
{
    if (initializeFileNode(name, 1))
    {
        printf("Directory '%s' created successfully\n", name);
    }
}

void createCmd(char *name)
{
    if (initializeFileNode(name, 0))
    {
        printf("File '%s' created Successfully\n", name);
    }
}

void lsCmd()
{
    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("(empty)\n");
    }
    else
    {
        do
        {
            printf("%s%s\n", temp->name, temp->isDirectory ? "/" : " ");
            temp = temp->next;
        } while (temp != cwd->child);
        printf("\n");
    }
}

int allocateBlock()
{
    int ans = -1;
    if (freeListHead)
    {
        FreeBlock *node = freeListHead;
        int idx = node->index;

        freeListHead = node->next;
        if (freeListHead)
        {
            freeListHead->prev = NULL;
        }
        else
        {
            freeListTail = NULL;
        }

        free(node);
        ans = idx;
    }

    return ans;
}

void freeBlock(int index)
{
    FreeBlock *newBlock = malloc(sizeof(FreeBlock));
    newBlock->index = index;
    newBlock->next = NULL;
    newBlock->prev = freeListTail;

    if (freeListTail)
    {
        freeListTail->next = newBlock;
    }
    else
    {
        freeListHead = newBlock;
    }

    freeListTail = newBlock;
}

int allocateBlocks(int required, int *outArr)
{
    int ans = 0;
    for (int index = 0; index < required; index++)
    {
        int idx = allocateBlock();
        if (idx == -1)
        {
            for (int j = 0; j < index; j++)
            {
                freeBlock(*(outArr + j));
            }
            return -1;
        }
        *(outArr + index) = idx;
    }
    return 0;
}

void writeCmd(char *name, char *data)
{
    FileNode *file = cwd->child;
    while (file && strcmp(file->name, name) != 0)
    {
        file = file->next;
        if (file == cwd->child)
            break;
    }
    if (!file || strcmp(file->name, name) != 0 || file->isDirectory)
    {
        printf("File '%s' not found \n", name);
    }
    else
    {
        int dataLength = strlen(data);
        int requiredBlocks = (dataLength + BLOCK_SIZE - 1) / BLOCK_SIZE;
        if (requiredBlocks == 0)
            requiredBlocks = 1;

        int *newBlocks = calloc(requiredBlocks, sizeof(int));
        if (!newBlocks)
        {
            printf("Memory error\n");
        }
        else
        {
            if (allocateBlocks(requiredBlocks, newBlocks) == -1)
            {
                printf("not enough free blocks to write '%s'\n", name);
                free(newBlocks);
            }
            else
            {
                int pos = 0;
                for (int index = 0; index < requiredBlocks; index++)
                {
                    int b = *(newBlocks + index);
                    for (int k = 0; k < BLOCK_SIZE && pos < dataLength; k++, pos++)
                    {
                        *(*(virtualDisk + b) + k) = *(data + pos);
                    }
                }

                if (file->blockPointers)
                {
                    for (int index = 0; index < file->blockCount; index++)
                    {
                        freeBlock(*(file->blockPointers + index));
                    }
                    free(file->blockPointers);
                }

                file->blockPointers = newBlocks;
                file->blockCount = requiredBlocks;
                file->size = dataLength;

                printf("Data written successfully (size=%d bytes) \n", dataLength);
            }
        }
    }
}

void readCmd(char *name)
{
    FileNode *file = cwd->child;
    if (!file)
    {
        printf("No files here. \n");
    }
    else
    {
        while (file && strcmp(name, file->name) != 0)
        {
            file = file->next;
            if (file == cwd->child)
                break;
        }

        if (!file || strcmp(file->name, name) != 0 || file->isDirectory)
        {
            printf("File '%s' not found\n", name);
        }
        else
        {
            if (file->blockCount == 0 || !file->blockPointers)
            {
                printf("File '%s' is empty \n", name);
            }
            else
            {

                int remaining = file->size;
                for (int i = 0; i < file->blockCount && remaining > 0; i++)
                {
                    int blockIndex = *(file->blockPointers + i);
                    for (int j = 0; j < BLOCK_SIZE && remaining > 0; j++, remaining--)
                    {
                        char ch = *(*(virtualDisk + blockIndex) + j);
                        putchar(ch);
                    }
                }

                printf("\n");
            }
        }
    }
}

void deleteCmd(char *name)
{
    FileNode *file = cwd->child;
    if (!file)
    {
        printf("No files to delete \n");
    }
    else
    {
        FileNode *head = file;
        FileNode *target = NULL;

        do
        {
            if (!file->isDirectory && strcmp(file->name, name) == 0)
            {
                target = file;
                break;
            }
            file = file->next;
        } while (file != head);

        if (!target)
        {
            printf("File '%s' not found \n", name);
        }
        else
        {
            if (target->blockPointers)
            {
                for (int i = 0; i < target->blockCount; i++)
                {
                    int blockIndex = *(target->blockPointers + i);
                    freeBlock(blockIndex);
                }
                free(target->blockPointers);
            }

            if (target->next == target)
            {
                cwd->child = NULL;
            }
            else
            {
                target->prev->next = target->next;
                target->next->prev = target->prev;
                if (cwd->child == target)
                {
                    cwd->child = target->next;
                }
            }

            free(target->name);
            free(target);

            printf("File deleted successfully \n");
        }
    }
}

void rmdirCmd(char *name)
{
    FileNode *file = cwd->child;
    if (!file)
    {
        printf("No directories here\n");
    }
    else
    {
        FileNode *head = file;
        FileNode *target = NULL;

        do
        {
            if (file->isDirectory && strcmp(file->name, name) == 0)
            {
                target = file;
                break;
            }
            file = file->next;
        } while (file != head);

        if (!target)
        {
            printf("Directory '%s' not found \n", name);
        }
        else
        {
            if (target->child)
            {
                printf("Directory '%s' is not empty \n", name);
            }
            else
            {
                if (target->next == target && target->prev == target)
                {
                    cwd->child = NULL;
                }
                else
                {
                    target->prev->next = target->next;
                    target->next->prev = target->prev;
                    if (cwd->child == target)
                    {
                        cwd->child = target->next;
                    }
                }
                free(target->name);
                free(target);

                printf("Directory '%s' removed successfully \n", name);
            }
        }
    }
}

void pwdCmd()
{
    if (!cwd)
    {
        printf("Error : currect working directory is NULL \n");
    }
    else
    {
        FileNode *temp = cwd;
        char *path = calloc(sizeof(char), 512);
        char *buffer = calloc(sizeof(char), 64);

        while (temp != NULL)
        {
            char *newPath = calloc(sizeof(char), 512);
            strcpy(newPath, "/");
            strcat(newPath, temp->name);
            strcat(newPath, path);
            strcpy(path, newPath);

            temp = temp->parent;
        }

        printf("current Path : %s\n", path[0] ? path : "/");
    }
}

void cdCmd(char *name)
{
    if (strcmp(name, "..") == 0)
    {
        if (cwd->parent != NULL)
        {
            cwd = cwd->parent;
        }
        printf("Moved to /%s\n", cwd->name);
    }
    else
    {
        FileNode *file = cwd->child;
        if (!file)
        {
            printf("NO subdirectories \n");
        }
        else
        {
            FileNode *head = file;
            do
            {
                if (file->isDirectory && strcmp(name, file->name) == 0)
                {
                    cwd = file;
                    printf("Moved to /%s\n", cwd->name);
                    break;
                }
                file = file->next;
            } while (file != head);
            if (strcmp(cwd->name, name) != 0)
            {
                printf("Directory '%s' not found \n", name);
            }
        }
    }
}

void dfCmd()
{
    int freeCount = 0;
    FreeBlock *file = freeListHead;
    while (file)
    {
        freeCount++;
        file = file->next;
    }

    int used = NUM_BLOCKS - freeCount;

    printf("Disk Status:\n");
    printf("Total Blocks : %d\n", NUM_BLOCKS);
    printf("Used  Blocks : %d\n", used);
    printf("Free  Blocks : %d\n", freeCount);
    printf("Disk Usage: %.2f%%\n", ((double)used / NUM_BLOCKS) * 100);
}

void freeFileTree(FileNode *node)
{
    if (node)
    {
        FileNode *child = node->child;
        if (child)
        {
            FileNode *start = child;
            do
            {
                FileNode *next = child->next;
                freeFileTree(child);
                child = next;
            } while (child != start);
        }

        if (node->blockPointers)
            free(node->blockPointers);
        if (node->name)
            free(node->name);
        free(node);
    }
}

void exitCmd()
{
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if (*(virtualDisk + i))
            free(*(virtualDisk + i));
    }
    free(virtualDisk);

    FreeBlock *fb = freeListHead;
    while (fb)
    {
        FreeBlock *next = fb->next;
        free(fb);
        fb = next;
    }

    freeFileTree(root);

    printf("Virtual File System closed. All memory freed.\n");
    exit(0);
}

CommandType getCommandType(char *input)
{
    if (strncmp(input, "mkdir", 5) == 0)
        return CMD_MKDIR;
    if (strncmp(input, "rmdir", 5) == 0)
        return CMD_RMDIR;
    if (strncmp(input, "cd", 2) == 0)
        return CMD_CD;
    if (strncmp(input, "pwd", 3) == 0)
        return CMD_PWD;
    if (strncmp(input, "create", 6) == 0)
        return CMD_CREATE;
    if (strncmp(input, "write", 5) == 0)
        return CMD_WRITE;
    if (strncmp(input, "read", 4) == 0)
        return CMD_READ;
    if (strncmp(input, "delete", 6) == 0)
        return CMD_DELETE;
    if (strncmp(input, "ls", 2) == 0)
        return CMD_LS;
    if (strncmp(input, "df", 2) == 0)
        return CMD_DF;
    if (strncmp(input, "exit", 4) == 0)
        return CMD_EXIT;
    return CMD_INVALID;
}

void showPrompt()
{
    if (cwd == root)
    {
        printf("/> ");
    }
    else
    {
        char *path = calloc(sizeof(char), 256);
        FileNode *temp = cwd;
        while (temp && temp->parent)
        {
            char *tmp = calloc(sizeof(char), 64);
            strcpy(tmp, "/");
            strcat(tmp, temp->name);

            char *newPath = calloc(sizeof(char), 256);
            strcpy(newPath, tmp);
            strcat(newPath, path);
            strcpy(path, newPath);

            temp = temp->parent;
        }
        printf("%s> ", path);
    }
}

int main()
{
    initializeDisk();

    char input[300];
    CommandType cmdType;

    while (1)
    {
        showPrompt();

        if (!fgets(input, sizeof(input), stdin))
            break;
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0)
            continue;

        char *cmd = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, "\0");

        if (!cmd)
            continue;

        cmdType = getCommandType(cmd);

        switch (cmdType)
        {

        case CMD_MKDIR:
            if (arg1)
                mkdirCmd(arg1);
            else
                printf("Usage: mkdir <dirname>\n");
            break;

        case CMD_RMDIR:
            if (arg1)
                rmdirCmd(arg1);
            else
                printf("Usage: rmdir <dirname>\n");
            break;

        case CMD_CD:
            if (arg1)
                cdCmd(arg1);
            else
                printf("Usage: cd <dirname> or cd ..\n");
            break;

        case CMD_PWD:
            pwdCmd();
            break;

        case CMD_CREATE:
            if (arg1)
                createCmd(arg1);
            else
                printf("Usage: create <filename>\n");
            break;

        case CMD_WRITE:
            if (arg1 && arg2)
            {
                if (arg2[0] == '\"' && arg2[strlen(arg2) - 1] == '\"')
                {
                    arg2[strlen(arg2) - 1] = 0;
                    arg2++;
                }
                writeCmd(arg1, arg2);
            }
            else
            {
                printf("Usage: write <filename> 'data'\n");
            }
            break;

        case CMD_READ:
            if (arg1)
                readCmd(arg1);
            else
                printf("Usage: read <filename>\n");
            break;

        case CMD_DELETE:
            if (arg1)
                deleteCmd(arg1);
            else
                printf("Usage: delete <filename>\n");
            break;

        case CMD_LS:
            lsCmd();
            break;

        case CMD_DF:
            dfCmd();
            break;

        case CMD_EXIT:
            exitCmd();
            return 0;

        default:
            printf("Unknown command. Try: mkdir, cd, create, write, read, delete, ls, rmdir, df, pwd, exit.\n");
            break;
        }

        printf("\n");
    }

    return 0;
}
