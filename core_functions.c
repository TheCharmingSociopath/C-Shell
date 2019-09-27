#include "headers.h"
int red_flag = 0, red_flag_append = 0, red_flag_inp = 0, num_bg = 0,
    flag_c = 0, flag_z = 0;
char *red_file, *red_file_append, *red_file_inp;

void wait_handler()
{
    int status;
    int i;
    for (i = 0; i < max_pid; ++i)
    {
        if (waitpid(i, &status, WNOHANG) && background_process[i][0] != '\0')
        {
            printf("\n%s with pid %d closed\n", background_process[i], i);
            printf("Press Enter to continue\n");
            background_process[i][0] = '\0';
            int temp = 0;
            for (int j = 0; j < num_bg; ++j)
            {
                if (background_process_pid_order[j] == i)
                    continue;
                background_process_pid_order[temp++] = background_process_pid_order[j];
            }
            --num_bg;
        }
    }
    return;
}

void signal_z(int sig)
{
    flag_z = 1;
    return;
}

void signal_c(int sig)
{
    flag_c = 1;
    return;
}

char *read_line()
{
    int position = 0, c;
    char *buffer = malloc(sizeof(char) * readline_buffersize);

    if (buffer == NULL)
    {
        fprintf(stderr, "Memory Error: Buffer can't be allocated\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        c = getchar();
        if (c == ';' || c == '\n' || c == EOF)
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
            buffer[position++] = c;
    }
}

char **split_line(char *line)
{
    int buffersize = 4096, position = 0;
    char **tokens = malloc(buffersize * sizeof(char *)), *token;

    if (tokens == NULL)
    {
        fprintf(stderr, "Memory Error: Buffer can't be allocated\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKENISE_DELIMITER);
    while (token != NULL)
    {
        tokens[position++] = token;
        token = strtok(NULL, TOKENISE_DELIMITER);
    }
    tokens[position] = NULL;
    return tokens;
}

char **handle_redirection(char **args)
{
    int position = 0, buffersize = 2048;
    char **new_args = malloc(buffersize * sizeof(char *));
    for (int i = 0; args[i] != NULL; ++i)
    {
        if (strcmp(args[i], ">") == 0)
        {
            red_file = args[++i];
            red_flag = 1;
            continue;
        }
        if (strcmp(args[i], "<") == 0)
        {
            red_file_inp = args[++i];
            red_flag_inp = 1;
            continue;
        }
        if (strcmp(args[i], ">>") == 0)
        {
            red_file_append = args[++i];
            red_flag_append = 1;
            continue;
        }
        new_args[position++] = args[i];
    }
    new_args[position] = NULL;
    return new_args;
}
char **split_pipe(char *line)
{
    int buffersize = 4096, position = 0;
    char **tokens = malloc(buffersize * sizeof(char *)), *token;

    if (tokens == NULL)
    {
        fprintf(stderr, "Memory Error: Buffer can't be allocated\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, "|");
    while (token != NULL)
    {
        tokens[position++] = token;
        token = strtok(NULL, "|");
    }
    tokens[position] = NULL;
    return tokens;
}
int execute_new(char **args)
{
    if (args[0] == NULL)
        return 1;
    int j = 0, fd[2], fdc = 0;
    char **new_args;
    if (args[1] == NULL)
    {
        new_args = split_line(args[0]);
        int ret = execute(new_args);
        return ret;
    }
    while (args[j] != NULL)
    {
        pipe(fd);

        if (fork() == 0)
        {
            dup2(fdc, 0);
            if (args[j + 1] != NULL)
            {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            new_args = split_line(args[j]);
            execute(new_args);
            free(new_args);
            exit(0);
        }
        else
        {
            wait(NULL);
            j++;
            fdc = dup(fd[0]);
            close(fd[1]);
        }
    }
    return 1;
}

int execute(char **args)
{
    args = handle_redirection(args);
    int ret, num_builtins = sizeof(builtin_str) / sizeof(char *), fd_stdin = dup(0), fd_stdout = dup(1);
    if (red_flag)
    {
        int fd = open(red_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if (fd < 0)
            perror("Error opening the file");
        if (dup2(fd, 1) < 0)
            perror("Error: failed at dup2");
        close(fd);
    }
    else if (red_flag_append)
    {
        int fd = open(red_file_append, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (fd < 0)
            perror("Error opening the file");
        if (dup2(fd, 1) < 0)
            perror("Error: failed at dup2");
        close(fd);
    }
    else if (red_flag_inp)
    {
        int fd = open(red_file_inp, O_RDONLY, 0);
        if (fd < 0)
            perror("Error opening the file");
        if (dup2(fd, 0) < 0)
            perror("Error: failed at dup2");
        close(fd);
    }
    for (int i = 0; i < num_builtins; ++i)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            ret = (*builtin_func[i])(args);
            dup2(fd_stdout, 1);
            dup2(fd_stdin, 0);
            close(fd_stdout);
            close(fd_stdin);
            red_flag = 0;
            red_flag_append = 0;
            red_flag_inp = 0;
            return ret;
        }
    }

    ret = launch(args);
    dup2(fd_stdout, 1);
    dup2(fd_stdin, 0);
    close(fd_stdout);
    close(fd_stdin);
    red_flag = 0;
    red_flag_append = 0;
    red_flag_inp = 0;
    return ret;
}

int launch(char **args)
{
    int i, background_flag = 0, status;
    for (i = 1; args[i] != NULL; ++i)
    {
        if (args[i][0] == '&')
        {
            background_flag = 1;
            args[i] = NULL;
            break;
        }
    }
    pid_t pid = fork(), wpid;
    if (pid == 0) // CHILD PROCESS
    {
        if (execvp(args[0], args) == -1)
        {
            fprintf(stderr, "Failed to execute the command through execvp.\n");
        }
        exit(EXIT_FAILURE); // execvp retuns => error
    }
    else if (pid < 0) // ERROR FORKING
    {
        perror("Too much forking caused an error. Slow down and look around.");
    }
    else if (pid > 0) // PARENT PROCESS
    {
        if (background_flag)
        {
            setpgid(pid, pid);
            signal(SIGCHLD, SIG_IGN);
            strcpy(background_process[pid], args[0]);
            background_process_pid_order[num_bg++] = pid;
        }
        else
        {
            flag_z = 0;
            flag_c = 0;
            signal(SIGTSTP, signal_z);
            signal(SIGINT, signal_c);
            while (!flag_z && !flag_c && waitpid(pid, &status, WNOHANG) != pid);
            if (flag_c)
            {
                kill(pid, 9);
                wait(NULL);
            }
            if (flag_z)
            {
                setpgid(pid, pid);
                kill(pid, SIGSTOP);
                strcpy(background_process[pid], args[0]);
                background_process_pid_order[num_bg++] = pid;
                signal(SIGCHLD, SIG_IGN);
            }
            flag_c = 0;
            flag_z = 0;
        }
    }
    return 1;
}

void manage_history(char *line)
{
    int i = 0;
    if (history[19][0] != '\0')
    {
        for (i = 0; i < 19; ++i)
        {
            strcpy(history[i], history[i + 1]);
        }
    }
    while (i < 20 && history[i][0] != '\0')
        ++i;
    strcpy(history[i], line);
    return;
    // strcpy(history[num_history++], line);
    // return;
}
