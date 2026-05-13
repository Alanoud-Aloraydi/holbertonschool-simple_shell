#include "shell.h"

/**
 * main - Entry point for the simple shell.
 * @ac: Argument count.
 * @av: Argument vector.
 *
 * Return: Always 0.
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *cmd;
	pid_t child_pid;
	int status;
	char *args[2];

	(void)ac;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);
		nread = getline(&line, &len, stdin);
		if (nread == -1) 
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			exit(EXIT_SUCCESS);
		}
		cmd = strtok(line, " \n\t\r"); 
		if (cmd == NULL)
			continue;
		child_pid = fork();
		if (child_pid == 0)
		{
			args[0] = cmd;
			args[1] = NULL;
			if (execve(args[0], args, environ) == -1)
			{
				perror(av[0]); 
				exit(EXIT_FAILURE);
			}
		}
		else
			wait(&status);
	}
	free(line);
	return (0);
}
