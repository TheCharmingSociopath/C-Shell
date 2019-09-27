#include "headers.h"

char **split_line_null(char *line)
{
    int buffersize = 4096, position = 0;
    char **tokens = malloc(buffersize * sizeof(char *)), *token;

    if (tokens == NULL)
    {
        fprintf(stderr, "Memory Error: Buffer can't be allocated\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, "\0");
    while (token != NULL)
    {
        tokens[position++] = token;
        token = strtok(NULL, "\0");
    }
    tokens[position] = NULL;
    return tokens;
}

int jobs(char **args)
{
    extern int num_bg;
    for (int i = 0; i < num_bg; ++i)
    {
        int id = background_process_pid_order[i], ret;
        printf("[%d] ", i + 1);
        char a[1024], b[1024], c[1024], filename[1024], procname[2048];
        sprintf(filename, "/proc/%d/stat", id);
        FILE *f = fopen(filename, "r");
        if (f)
        {
            fscanf(f, "%s %s %s", a, b, c);
            fclose(f);
        }
        else
        {
            perror("Error opening the proc file. Try again.");
            return 1;
        }
        if (!strcmp(c, "T"))
            printf("Stopped ");
        else
            printf("Running ");
        sprintf(filename, "/proc/%d/cmdline", id);
        f = fopen(filename, "r");
        if (f)
        {
            fscanf(f, "%s", procname);
            printf("%s ", procname);
            // while (fscanf(f, "%s", procname) != EOF)
            // {
            //     char **temp = split_line_null(procname);
            //     int t = 0;
            //     while (temp[t] != NULL)
            //         printf("%s ", temp[t++]);
            // }
            // printf("%s ", procname);
            // char **temp = split_line_null(procname);
            fclose(f);
        }
        else
        {
            perror("Error opening the proc file. Try again.");
            return 1;
        }
        printf("[%d]\n", id);
    }
    return 1;
}
