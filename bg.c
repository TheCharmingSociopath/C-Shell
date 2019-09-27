#include "headers.h"

int bg(char **args)
{
    if (args[1] == NULL || args[2] != NULL)
    {
        perror("Invalid number of arguments.\nSyntax: bg <job_number>\n");
        return 1;
    }
    extern int num_bg;
    int job_number = atoi(args[1]);
    if (job_number > num_bg)
    {
        perror("Invalid job number\n");
        return 1;
    }
    int process_id = background_process_pid_order[job_number - 1];
    kill(process_id, SIGCONT);
    return 1;
}
