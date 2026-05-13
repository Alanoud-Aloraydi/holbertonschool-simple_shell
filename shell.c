#include "shell.h"

/**
 * handle_ctrl_c - handles Ctrl+C signal
 * @sig: signal number
 */
void handle_ctrl_c(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}

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
	char *args[1024];
	char *cmd;
	int i, status;
	pid_t child_pid;

	(void)ac;
	signal(SIGINT, handle_ctrl_c);

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

		i = 0;
		token = strtok(line, " \n\t\r");
		while (token)
		{
			args[i++] = token;
			token = strtok(NULL, " \n\t\r");
		}
		args[i] = NULL;

		if (args[0] == NULL)
			continue;

		if (strchr(args[0], '/'))
			cmd = args[0];
		else
			cmd = get_path(args[0]);

		if (cmd == NULL)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", av[0], args[0]);
			continue;
		}

		child_pid = fork();
		if (child_pid == 0)
		{
			execve(cmd, args, environ);
			exit(127);
		}
		else
			wait(&status);
	}

	free(line);
	return (0);
}
