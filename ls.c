#include "headers.h"

int l = 0, a = 0;

int ls(char **args)
{
    int dir_count = 0;
    char directories[100][128];
    for (int i = 1; args[i] != NULL; ++i)
    {
        if (args[i][0] == '-')
        {
            if (args[i][1] == '\0')
            {
                strcpy(directories[dir_count++], "-");
                continue;
            }
            else if (args[i][1] == 'l')
                l = 1;
            else if (args[i][1] == 'a')
                a = 1;
            if (args[i][2] != '\0')
            {
                if (args[i][2] == 'l')
                    l = 1;
                else if (args[i][2] == 'a')
                    a = 1;
            }
        }
        else
        {
            if (args[i][0] == '~' && args[i][1] == '\0')
                strcpy(directories[dir_count++], present_root);
            else
                strcpy(directories[dir_count++], args[i]);
        }
    }
    if (dir_count == 0)
    {
        ++dir_count;
        strcpy(directories[0], cwd);
    }
    for (int i = 0; i < dir_count; ++i)
    {
        if (dir_count > 1)
            printf("%s:\n", directories[i]);
        print_ls(directories[i]);
    }
    l = 0;
    a = 0;
    return 1;
}
void print_ls(char *dname)
{
    int num_entry;
    struct stat st;
    struct dirent **entries;
    DIR *d = opendir(dname);
    if (d == NULL)
    {
        fprintf(stderr, "Error opening the directory %s\n\n", dname);
    }
    char path[1024];
    strcpy(path, cwd);
    strcpy(path, dname);
    num_entry = scandir(path, &entries, NULL, alphasort);
    chdir(path);

    for (int i = 0; i < num_entry; i++)
    {
        if ((entries[i]->d_name)[0] == '.' && a == 0)
            continue;
        if (l == 0)
        {
            printf("%s ", entries[i]->d_name);
        }
        else
        {
            stat(entries[i]->d_name, &st);
            struct passwd *owner = getpwuid(st.st_uid);
            struct group *group = getgrgid(st.st_gid);
            char *date = malloc(256);
            strftime(date, 256, "%b\t%d\t%H:%M", localtime(&st.st_mtime));
            printf("%s\t%ld\t%s\t%s\t%ld\t%s\t%s\n", permissions(entries[i]->d_name), st.st_nlink,
                   owner->pw_name, group->gr_name, st.st_size, date, entries[i]->d_name);
        }
    }
    printf("\n");
    chdir(cwd);
    closedir(d);
}

char *permissions(char *filename)
{
    // char template[] = "-rwxrwxrwx";
    char permission[] = "----------";
    struct stat st;
    // mode_t mode = st.st_mode;
    if (stat(filename, &st) != 0)
    {
        perror("There was an error during reading permissions.\n");
        return strdup(&permission[0]);
    }

    if (S_ISREG(st.st_mode))
        permission[0] = '-';
    else if (S_ISDIR(st.st_mode))
        permission[0] = 'd';
    else if (S_ISLNK(st.st_mode))
        permission[0] = 'l';
    else if (S_ISBLK(st.st_mode))
        permission[0] = 'b';
    else if (S_ISCHR(st.st_mode))
        permission[0] = 'c';
    else if (S_ISFIFO(st.st_mode))
        permission[0] = '|';

    permission[1] = st.st_mode & S_IRUSR ? 'r' : '-';
    permission[2] = st.st_mode & S_IWUSR ? 'w' : '-';
    permission[3] = st.st_mode & S_IXUSR ? 'x' : '-';
    permission[4] = st.st_mode & S_IRGRP ? 'r' : '-';
    permission[5] = st.st_mode & S_IWGRP ? 'w' : '-';
    permission[6] = st.st_mode & S_IXGRP ? 'x' : '-';
    permission[7] = st.st_mode & S_IROTH ? 'r' : '-';
    permission[8] = st.st_mode & S_IWOTH ? 'w' : '-';
    permission[9] = st.st_mode & S_IXOTH ? 'x' : '-';
    // for (int i = 9; i > 0; --i)
    // {
    //     if (mode % 2)
    //         permission[i] = template[i];
    //     mode /= 2;
    // }
    return strdup(&permission[0]);
}
