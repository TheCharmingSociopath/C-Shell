#include "headers.h"

int main()
{
    char *line, **args;
    int status = 1, i = 0, cwd_size, present_root_size;

    for (i = 0; i < max_pid; ++i)
        background_process[i][0] = '\0';

    if (getcwd(present_root, sizeof(present_root)) != NULL)
        present_root_size = strlen(present_root);
    else
        perror("getcwd() error");
    while (status)
    {
        signal(SIGCHLD, wait_handler);

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
        manage_history(line);
        args = split_line(line);
        status = execute(args); // 1 == Success. Prompt again
        free(line);
        free(args);
    }
    return EXIT_SUCCESS;
}
