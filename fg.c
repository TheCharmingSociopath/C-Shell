#include "headers.h"
extern void signal_z(int sig);
extern void signal_c(int sig);

int fg(char **args)
{
    extern int flag_c;
    extern int flag_z;
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
    flag_z = 0;
    flag_c = 0;
    signal(SIGTSTP, signal_z);
    signal(SIGINT, signal_c);
    while (!flag_z && !flag_c && waitpid(process_id, &status, WNOHANG) != process_id)
        ;
    if (flag_c)
    {
        kill(process_id, 9);
        wait(NULL);
    }
    if (flag_z)
    {
        setpgid(process_id, process_id);
        kill(process_id, SIGSTOP);
        strcpy(background_process[process_id], args[0]);
        background_process_pid_order[num_bg++] = process_id;
        signal(SIGCHLD, SIG_IGN);
    }
    flag_c = 0;
    flag_z = 0;
    // do
    // {
    //     wpid = waitpid(process_id, &status, WUNTRACED);
    // } while (WIFEXITED(status) == 0 && WIFSIGNALED(status) == 0);
    return 1;
}
