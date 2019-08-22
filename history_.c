#include "headers.h"

int history_(char **args)
{
    int count;
    if (args[1] == NULL)
        count = 10;
    else
        count = atoi(args[1]);
    if (args[2] != NULL)
    {
        fprintf(stderr, "Too many arguments to \"history.\" Expected 1 argument.\n");
        return 1;
    }
    for (int i = 0; i < count; ++i)
    {
        if (history[i][0] == '\0')
            break;
        printf("%s\n", history[i]);
    }
    return 1;
}
