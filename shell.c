#include "shell.h"

/**
 * main - UNIX command line interpreter that handles arguments.
 * @ac: Argument count.
 * @av: Argument vector.
 *
 * Return: Always 0.
 */
int main(int ac, char **av)
{
	char *line = NULL, *token;
	size_t len = 0;
	ssize_t nread;
	char *args[1024]; /* Array to store command and arguments */
	char *cmd;
	int i, status;
	pid_t child_pid;

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
		/* Tokenize input into args array */
		i = 0;
		token = strtok(line, " \n\t\r");
		while (token != NULL)
		{
			args[i++] = token;
			token = strtok(NULL, " \n\t\r");
		}
		args[i] = NULL;
		if (args[0] == NULL)
			continue;
		cmd = get_path(args[0]);

                if (cmd == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "#cisfun$ ", 9);
         		continue;
		}
		child_pid = fork();
		if (child_pid == 0)
		{
			if (execve(cmd, args, environ) == -1)
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
