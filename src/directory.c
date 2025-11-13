#include "file.h"
#include "commandParser.h"

void initializeRootDirectory()
{
    root = (FileNode *)malloc(sizeof(FileNode));
    if (!root)
    {
        printf("Memory allocation failed for root directory.\n");
        exit(1);
    }

    root->name = (char *)malloc(50 * sizeof(char));
    strcpy(root->name, " ");
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

bool doesDirectoryExist(const char *name)
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

bool initializeFileNode(const char *name, const int isDirectory)
{
    bool ans = false;
    if (!doesDirectoryExist(name))
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

void makeDirectory(const char *name)
{
    if (initializeFileNode(name, 1))
    {
        printf("Directory '%s' created successfully\n", name);
    }
}

void removeDirectory(const char *name)
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

void showDirectories()
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

void changeDirectory(const char *name)
{
    if (strcmp(name, "..") == 0)
    {
        if (cwd->parent != NULL)
        {
            cwd = cwd->parent;
        }
        if (cwd->parent == NULL)
        {
            printf("Moved to /\n");
        }
        else
        {
            printf("Moved to /%s\n", cwd->name);
        }
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

void showCurrentPath()
{
    if (!cwd)
    {
        printf("Error : currect working directory is NULL \n");
    }
    else
    {
        FileNode *temp = cwd;
        char *path = calloc(sizeof(char), 512);

        while (temp->parent != NULL)
        {
            char *newPath = calloc(sizeof(char), 512);
            strcpy(newPath, "/");
            strcat(newPath, temp->name);
            strcat(newPath, path);
            strcpy(path, newPath);

            temp = temp->parent;
        }

        printf("current Path : %s\n", path[0] ? path : "");
    }
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
        {
            free(node->blockPointers);
        }
        if (node->name)
        {
            free(node->name);
        }
        free(node);
    }
}

void exitProgram()
{
    for (int index = 0; index < NUM_BLOCKS; index++)
    {
        if (*(virtualDisk + index))
        {
            free(*(virtualDisk + index));
        }
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
