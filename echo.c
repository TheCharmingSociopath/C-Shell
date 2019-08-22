#include "headers.h"

int echo(char **args)
{
    if (args[1] == NULL)
    {
        printf("\n");
        return 1;
    }
    int quote = 0, lines = 0, size = 0;
    char **print_string = malloc(sizeof(char) * readline_buffersize);
    for (int i = 1; args[i] != NULL; ++i)
    {
        // if (args[i][0] == '\'' || args[i][0] == '\"')
        //     quote = 1;
        printf("%s ", args[i]);
    }
    printf("\n");
    return 1;
}
