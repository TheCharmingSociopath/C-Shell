#include "headers.h"

void wait_handler()
{
    int status;
    int i;
    for (i = 0; i < 100; ++i)
    {
        if (waitpid(i, &status, WNOHANG) && background_process[i][0] != '\0')
        {
            printf("\n%s with pid: %d exited normally\n", background_process[i], i);
            printf("Press Enter to continue\n");
            background_process[i][0] = '\0';
        }
    }
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

int execute(char **args)
{
    if (args[0] == NULL)
        return 1;
    int num_builtins = sizeof(builtin_str) / sizeof(char *);
    for (int i = 0; i < num_builtins; ++i)
        if (strcmp(args[0], builtin_str[i]) == 0)
            return (*builtin_func[i])(args);

    return launch(args);
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
            strcpy(background_process[pid], args[0]);
        else
        {
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (WIFEXITED(status) == 0 && WIFSIGNALED(status) == 0);
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
    return 1;
}
