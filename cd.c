#include "headers.h"

int cd(char **args)
{
    char *arg = (args[1] == NULL) ? present_root : args[1];
    if (args[2] != NULL)
        fprintf(stderr, "Too many arguments to \"cd.\" Expected 1 argument.\n");
    else
    {
        if (chdir(arg) != 0)
        {
            perror("There wan an error in executing chdir.\n");
        }
    }
    return 1;
}
