#include "headers.h"

int fg(char **args)
{
    if (args[1] == NULL || args[2] != NULL)
    {
        perror("Invalid number of arguments.\nSyntax: fg <job_number>\n");
        return 1;
    }
    extern int num_bg;
    int job_number = atoi(args[1]);
    if (job_number > num_bg)
    {
        perror("Invalid job number\n");
        return 1;
    }
    pid_t wpid;
    int process_id = background_process_pid_order[job_number - 1], status;
    kill(process_id, SIGCONT);
    do
    {
        wpid = waitpid(process_id, &status, WUNTRACED);
    } while (WIFEXITED(status) == 0 && WIFSIGNALED(status) == 0);
    return 1;
}
