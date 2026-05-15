#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Environment array matrix */
extern char **environ;

/* Function prototypes */
int main(int ac, char **av);
char *get_path(char *command);
void handle_ctrl_c(int sig);
void free_env_allocs(void);
void update_env(char *name, char *value);
char *get_env_value(char *name);
int builtin_cd(char **args, char *av0, int line_count);
void builtin_env(void);

#endif /* SHELL_H */
