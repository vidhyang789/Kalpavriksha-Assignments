#include "../inc/commandParser.h"

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