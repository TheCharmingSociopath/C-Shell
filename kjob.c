#include "headers.h"


int kjob(char **args)
{
    extern int num_bg;
    if (args[1] == NULL || args[2] == NULL || args[3] != NULL)
    {
        perror("Invalid number of arguments.\nSyntax: kjob <job_number> <signal_number>\n");
        return 1;
    }
    int job_number = atoi(args[1]), signal = atoi(args[2]);
    int process_id = background_process_pid_order[job_number - 1];
    if (job_number > num_bg)
    {
        perror("Error: Invalid Job number.\n");
        return 1;
    }
        kill(process_id, signal);
        if (signal == 9)
        {
            int temp = 0;
            for (int j = 0; j < num_bg; ++j)
            {
                if (background_process_pid_order[j] == process_id)
                    continue;
                background_process_pid_order[temp++] = background_process_pid_order[j];
            }
            background_process[process_id][0] = '\0';
            --num_bg;
        }
        return 1;
}
