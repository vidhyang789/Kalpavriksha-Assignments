#include "file.h"
#include "commandParser.h"

char **virtualDisk;
FreeBlock *freeListHead = NULL;
FreeBlock *freeListTail = NULL;

FileNode *root = NULL;
FileNode *cwd = NULL;

void initializeVirtualBlock()
{
    virtualDisk = (char **)malloc(NUM_BLOCKS * sizeof(char *));
    for (int index = 0; index < NUM_BLOCKS; index++)
    {
        virtualDisk[index] = (char *)calloc(BLOCK_SIZE, sizeof(char));
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
            for (int index2 = 0; index2 < index; index2++)
            {
                freeBlock(*(outArr + index2));
            }
            return -1;
        }
        *(outArr + index) = idx;
    }
    return 0;
}

void createFile(char *name)
{
    if (initializeFileNode(name, 0))
    {
        printf("File '%s' created Successfully\n", name);
    }
}

void writeFile(char *name, char *data)
{
    FileNode *file = cwd->child;
    while (file && strcmp(file->name, name) != 0)
    {
        file = file->next;
        if (file == cwd->child)
        {
            break;
        }
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
        {
            requiredBlocks = 1;
        }

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
                    int idx = *(newBlocks + index);
                    for (int k = 0; k < BLOCK_SIZE && pos < dataLength; k++, pos++)
                    {
                        *(*(virtualDisk + idx) + k) = *(data + pos);
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

void readFile(char *name)
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
            {
                break;
            }
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
                for (int index = 0; index < file->blockCount && remaining > 0; index++)
                {
                    int blockIndex = *(file->blockPointers + index);
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

void deleteFile(char *name)
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
                for (int index = 0; index < target->blockCount; index++)
                {
                    int blockIndex = *(target->blockPointers + index);
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

void showDiskDetials()
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
    printf("Disk  Usage: %.2f%%\n", ((double)used / NUM_BLOCKS) * 100);
}
