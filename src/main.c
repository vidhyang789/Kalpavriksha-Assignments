#include "../inc/virtualFileSystem.h"

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

}
