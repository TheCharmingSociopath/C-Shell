#include "headers.h"

int main()
{
    char *line, **args;
    int status = 1, i = 0, cwd_size, present_root_size;

    for (i = 0; i < max_pid; ++i)
        background_process[i][0] = '\0';
    for (i = 0; i < 20; ++i)
        history[i][0] = '\0';
    extern int num_up;

    if (getcwd(present_root, sizeof(present_root)) != NULL)
        present_root_size = strlen(present_root);
    else
        perror("getcwd() error");
    while (status)
    {
        signal(SIGCHLD, wait_handler);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, SIG_IGN);

        if (getlogin_r(username, sizeof(username)))
            perror("getlogin_r() error");

        if (gethostname(hostname, sizeof(hostname)))
            perror("gethostname() error");

        if (getcwd(cwd, sizeof(cwd)) != NULL)
            cwd_size = strlen(cwd);
        else
            perror("getcwd() error");

        printf("\x1b[0;30;42m%s@%s\x1b[0;30;42m:", username, hostname);
        if (cwd_size >= strlen(present_root))
        {
            printf("~");
            for (i = strlen(present_root); i < cwd_size; ++i)
                printf("%c", cwd[i]);
            printf("\x1b[0m ");
        }
        else
            printf("%s>\033[0m\x1b[0m ", cwd);

        line = read_line();
        if (num_up > 0)
        {
            char *line1;
            printf("\x1b[0;30;42m%s@%s\x1b[0;30;42m:", username, hostname);
            if (cwd_size >= strlen(present_root))
            {
                printf("~");
                for (i = strlen(present_root); i < cwd_size; ++i)
                    printf("%c", cwd[i]);
                printf("\x1b[0m ");
            }
            else
                printf("%s>\033[0m\x1b[0m ", cwd);
            printf("%s ", line);
            num_up = 0;
            line1 = read_line();
            strcat(line, " ");
            strcat(line, line1);
        }
        manage_history(line);
        args = split_pipe(line);
        status = execute_new(args); // 1 == Success. Prompt again
    }
    return EXIT_SUCCESS;
}
