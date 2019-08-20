#include <stdio.h>
#include <stdlib.h>
#include "headers.h"
#include <signal.h>

_Bool get_flag;
char back_process[1<<20][256];

int main(int argc, char **argv)
{
    	get_flag = 1;
	char *line;
	char **args;
	int status;
	int i = 0;

	for(i=0;i < 1<<20; ++i) back_process[i][0] = '\0';
	
	char present_root[PATH_MAX];
	char usr[128];
	char host[128];
	char cwd[256];

	if(getcwd(present_root, sizeof(present_root)) == NULL) {
		perror("getcwd() error");
	}
	int present_root_size = 0;
	int cwd_size = 0;
	while(present_root[present_root_size++]!='\0');

	do {

		signal(SIGCHLD, wait_handler);

		if(get_flag) {
			if(getcwd(cwd, sizeof(cwd)) == NULL) {
				perror("getcwd() error");
			}
			cwd_size = 0;
			while(cwd[cwd_size++] != '\0');

			if(getlogin_r(usr, sizeof(usr))) {
				perror("getlogin_r() error");
			}

			if(gethostname(host, sizeof(host))) {
				perror("gethostname() error");
			}

			printf("\033[1;32m<%s@%s\033[0m:\033[1;34m", usr, host);
			if(cwd_size >= present_root_size) {
				printf("~");
				for(i = present_root_size;i < cwd_size; ++i) {
					printf("%c", cwd[i]);
				}
				printf(">\033[0m");
			}
			else {
				printf("%s>\033[0m",cwd);
			}
		}
		if(!get_flag)
			get_flag = 1;
		line = read_line();
		args = split_line(line);
		status = execute(args);

		free(line);
		free(args);


	} while(status);

	return EXIT_SUCCESS;
}

void dmh(void)
{

}
