#include "headers.h"

int unsetenv_(char **args)
{
    if(args[1] == NULL || args[2] != NULL)
    {
        perror("Invalid number of arguments for unsetenv. \nSyntax : unsetenv var");
        return 1;
    }
    if(unsetenv(args[1]) != 0)
        perror("Error while trying to remove the environment variable.");
    return 1;
}
