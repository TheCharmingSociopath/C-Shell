#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>

#define TOKENISE_DELIMITER " \t\r\n\a"
#define readline_buffersize 4096
#define max_pid 1<<20

char background_process[max_pid][1024], present_root[256], username[256],
hostname[256], cwd[256], history[20][4096];
int background_process_pid_order[max_pid];

// Core Functions
void wait_handler();
char *read_line();
char **split_line(char *line);
char **handle_redirection(char **args);
char **split_pipe(char *line);
int execute(char **args);
int execute_new(char **args);
int launch(char **args);

// Support functions
int alphasort();
void print_ls(char *dname);
char *permissions(char *filename);
void manage_history(char *line);

// Custom commands
int cd(char **args);
int pwd(char **args);
int my_exit(char **args);
int echo(char **args);
int pinfo(char **args);
int ls(char **args);
int history_(char **args);
int nightswatch(char **args);
int setenv_(char **args);
int unsetenv_(char **args);
int jobs(char **args);
int kjob(char **args);
int fg(char **args);
int bg(char **args);

static const char *builtin_str[] = {
	"cd",
	"pwd",
	"quit",
	"echo",
	"pinfo",
	"ls",
	"history",
	"nightswatch",
	"setenv",
	"unsetenv",
	"jobs",
	"kjob",
	"fg",
	"bg",
};

static const int (*builtin_func[]) (char **) = {
	&cd,
	&pwd,
	&my_exit,
	&echo,
	&pinfo,
	&ls,
	&history_,
	&nightswatch,
	&setenv_,
	&unsetenv_,
	&jobs,
	&kjob,
	&fg,
	&bg,
};
