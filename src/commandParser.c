#include "commandParser.h"

CommandType getCommandType(const char *input)
{
    if (strncmp(input, "mkdir", 5) == 0)
    {
        return MKDIR;
    }
    if (strncmp(input, "rmdir", 5) == 0)
    {
        return RMDIR;
    }
    if (strncmp(input, "cd", 2) == 0)
    {
        return CD;
    }
    if (strncmp(input, "pwd", 3) == 0)
    {
        return PWD;
    }
    if (strncmp(input, "create", 6) == 0)
    {
        return CREATE;
    }
    if (strncmp(input, "write", 5) == 0)
    {
        return WRITE;
    }
    if (strncmp(input, "read", 4) == 0)
    {
        return READ;
    }
    if (strncmp(input, "delete", 6) == 0)
    {
        return DELETE;
    }
    if (strncmp(input, "ls", 2) == 0)
    {
        return LS;
    }
    if (strncmp(input, "df", 2) == 0)
    {
        return DF;
    }
    if (strncmp(input, "exit", 4) == 0)
    {
        return EXIT;
    }
    return INVALID;
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
