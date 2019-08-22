#include "headers.h"

int pwd(char **args)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != 0)
        perror("There wan an error in executing getpwd.\n");
    else
        printf("%s\n", cwd);
    return 1;
}
