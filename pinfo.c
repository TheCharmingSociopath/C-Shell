#include "headers.h"

int pinfo(char **args)
{
    if (args[2] != NULL)
    {
        fprintf(stderr, "Too many arguments to \"pinfo.\" Expected 1 argument.\n");
        return 1;
    }
    char path[1024] = "/proc/", str[2048], exec_path[2048], buffer[2048];
    if (args[1] == NULL)
        strcat(path, "self");
    else
        strcat(path, args[1]);
    strcpy(exec_path, path);
    strcat(exec_path, "/exe");
    strcat(path, "/stat");
    FILE *fd = fopen(path, "r");
    if (fd == NULL)
    {
        fprintf(stderr, "Process ID seems to be wrong.\n");
        return 1;
    }
	if (readlink(exec_path, buffer, sizeof(buffer)) < 0)
    {
        fprintf(stderr, "Could not obtain executeable path\n");
		return 1;
    }
    for (int i = 0; i < 23; ++i)
    {
        fscanf(fd, "%s", str);
        if (i == 0)
            printf("pid -- %s\n", str);
        else if (i == 2)
            printf("Process Status -- %s\n", str);
        else if (i == 22)
            printf("memory -- %s\n", str);
    }
    printf("Executable Path -- %s\n", buffer);
    return 1;
}
