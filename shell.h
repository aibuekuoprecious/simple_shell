#ifndef _SHELL_H_
#define _SHELL_H_

/* System Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* Defines the size of read and write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024

/* The buffer is flushed after each write operation */
#define BUF_FLUSH -1

/* Defines the different types of command chaining supported */
#define CMD_NORM 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* Constants used to converts a string to a number */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* Determines which function to use for parsing input */
#define USE_GETLINE 0
#define USE_STRTOK 0

/* Constants used for command history functionality */
#define HISTORY_FILE ".shell_history"
#define MAX_HISTORY 4096

/*Global variable*/
extern char **environ;

/**
 * struct sinlist - singly linked list
 * @num: store numerical information
 * @str: points to a string
 * @next: points to the next node
 */
typedef struct sinlist
{
	int num;
	char *str;
	struct sinlist *next;
} linked_list;

/**
 * struct shell_info - a set of parameters used for passing arguments into a function in a uniform way
 * this allows for the creation of a function pointer struct that can take this struct as a parameter.
 * @arg: a string generated from getline containing arguements
 * @argv:an array of strings generated from `arg`
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @history_num: the history line number count
 */
typedef struct shell_info
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	linked_list *env;
	linked_list *history;
	linked_list *alias;
	char **environ;
	int env_changed;
	int status;
	char **cmd_buf;
	int cmd_buf_type;
	int readfd;
	int history_num;
} data;

/*  Defines an initializer for the data struct */
#define DATA_INIT                                                                       \
	{                                                                                   \
		NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0 \
	}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(data *);
} builtin_table;

/* core.c */
int read_input(data *, char **);
int find_builtin(data *);
void find_cmd(data *);
void fork_cmd(data *);

/* state.c */
int is_cmd(data *, char *);
char *dup_chars(char *, int, int);
char *find_path(data *, char *, char *);

/* fdout.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* strings.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* strout.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* strargs.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* tokenize.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* memory.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);
int bfree(void **);

/* helpers.c */
int interactive(data *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* data.c */
int _erratoi(char *);
void print_error(data *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* builtin.c */
int _myexit(data *);
int _mycd(data *);
int _myhelp(data *);

/* builtins.c */
int _myhistory(data *);
int _myalias(data *);

/* inout.c */
ssize_t get_input(data *);
int _getline(data *, char **, size_t *);
void sigintHandler(int);

/* struct.c */
void clear_info(data *);
void set_info(data *, char **);
void free_info(data *, int);

/* env.c */
int _myenv(data *);
char *_getenv(data *, const char *);
int _mysetenv(data *);
int _myunsetenv(data *);
int populate_env_list(data *);

/* envpro.c */
char **get_environ(data *);
int _unsetenv(data *, char *);
int _setenv(data *, char *, char *);

/* history.c */
char *get_history_file(data *info);
int write_history(data *info);
int read_history(data *info);
int build_history(data *info, char *buf, int linecount);
int renumber_history(data *info);

/* lists.c */
linked_list *add_node(linked_list **, const char *, int);
linked_list *add_node_end(linked_list **, const char *, int);
size_t print_list(const linked_list *);
int rm_node(linked_list **, unsigned int);
void free_list(linked_list **);

/* sinlist.c */
size_t list_len(const linked_list *);
char **list_to_strings(linked_list *);
size_t print_list(const linked_list *);
linked_list *node_starts_with(linked_list *, char *, char);
ssize_t node_index(linked_list *, linked_list *);

/* cmdin.c */
int is_chain(data *, char *, size_t *);
void check_chain(data *, char *, size_t *, size_t, size_t);
int replace_alias(data *);
int replace_vars(data *);
int replace_string(char **, char *);

#endif /* _SHELL_H_ */
