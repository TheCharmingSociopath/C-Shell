#define DMH_PATH_MAX 128
#define DMH_RL_BUFSIZE 1024
#define DMH_TOK_BUFSIZE 64
#define DMH_ECHO_MAX 1024
#define DMH_TOK_DELIM " \t\r\n\a"

extern char back_process[1<<20][256];
extern _Bool get_flag;
extern int alphasort();

// dmh shell main
void dmh(void);
char *dmh_read_line(void);
char **dmh_split_line(char *line);
int dmh_execute(char** args);
int dmh_launch(char **args);

// builtin declarations 
int dmh_exit(char **args);
int dmh_cd(char **args);
int dmh_pwd(char **args);
int dmh_echo(char **args);
int dmh_ls(char **args);
int dmh_pinfo(char **args);
int dmh_remindme(char **args);
int dmh_clock(char **args);

// list of commands
static const char *builtin_str[] = {
	"exit",
	"cd",
	"pwd",
	"echo",
	"ls",
	"pinfo",
	"remindme",
	"clock"
};

static const int (*builtin_func[]) (char **) = {
	&dmh_exit,
	&dmh_cd,
	&dmh_pwd,
	&dmh_echo,
	&dmh_ls,
	&dmh_pinfo,
	&dmh_remindme,
	&dmh_clock
};

// extra functions
void dmh_wait_handler();
int dmh_num_builtins();
