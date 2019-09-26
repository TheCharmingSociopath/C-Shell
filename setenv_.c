#include "headers.h"

int setenv_(char **args)
{
    if(args[1] == NULL || args[3] != NULL)
    {
        perror("Invalid number of arguments.\nSyntax: setenv var [value]");
        return 1;
    }
    if(setenv(args[1], args[2], 1) != 0)
        perror("Error while trying to create or modify the specified environment variable.");
    return 1;
}
