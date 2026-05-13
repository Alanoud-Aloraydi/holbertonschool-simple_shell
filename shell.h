#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* environment variable */
extern char **environ;

int main(int ac, char **av);
char *get_path(char *command);

#endif /* SHELL_H */
