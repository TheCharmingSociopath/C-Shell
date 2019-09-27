#include "headers.h"

int overkill(char **args)
{
    if (args[1] != NULL)
    {
        perror("Invalid number of arguments.\nSyntax: overkill\n");
        return 1;
    }
    extern int num_bg;
    for (int i = 0; i < num_bg; ++i)
    {
        int pid = background_process_pid_order[i];
        kill(pid, 9);
        background_process[pid][0] = '\0';
        background_process_pid_order[i] = 0;
    }
    num_bg = 0;
    return 1;
}
