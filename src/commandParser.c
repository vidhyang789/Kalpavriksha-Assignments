#include "commandParser.h"

CommandType getCommandType(char *input)
{
    if (strncmp(input, "mkdir", 5) == 0)
    {
        return CMD_MKDIR;
    }
    if (strncmp(input, "rmdir", 5) == 0)
    {
        return CMD_RMDIR;
    }
    if (strncmp(input, "cd", 2) == 0)
    {
        return CMD_CD;
    }
    if (strncmp(input, "pwd", 3) == 0)
    {
        return CMD_PWD;
    }
    if (strncmp(input, "create", 6) == 0)
    {
        return CMD_CREATE;
    }
    if (strncmp(input, "write", 5) == 0)
    {
        return CMD_WRITE;
    }
    if (strncmp(input, "read", 4) == 0)
    {
        return CMD_READ;
    }
    if (strncmp(input, "delete", 6) == 0)
    {
        return CMD_DELETE;
    }
    if (strncmp(input, "ls", 2) == 0)
    {
        return CMD_LS;
    }
    if (strncmp(input, "df", 2) == 0)
    {
        return CMD_DF;
    }
    if (strncmp(input, "exit", 4) == 0)
    {
        return CMD_EXIT;
    }
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
