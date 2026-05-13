#include "shell.h"

/**
 * main - Entry point for a simple UNIX command line interpreter.
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
	pid_t child_pid;
	int status;
	char *args[2];

	(void)ac;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("#cisfun$ ");
		nread = getline(&line, &len, stdin);
		if (nread == -1) /* Handle EOF (Ctrl+D) */
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error");
			continue;
		}
		if (child_pid == 0)
		{
			args[0] = line;
			args[1] = NULL;
			if (execve(args[0], args, environ) == -1)
				perror(av[0]);
			exit(EXIT_FAILURE);
		}
		else
			wait(&status);
	}
	free(line);
	return (0);
}
