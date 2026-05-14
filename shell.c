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
	int last_status = 0;
	int line_count = 1;
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
			free_env_allocs();
			exit(last_status);
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
		if (strcmp(args[0], "exit") == 0)
		{
    			free(line);
			free_env_allocs();
    			exit(last_status);
		}
		if (strcmp(args[0], "cd") == 0)
		{
    			last_status = builtin_cd(args, av[0], line_count);
    			line_count++;
    			continue;
		}

		if (strchr(args[0], '/'))
			cmd = args[0];
		else
			cmd = get_path(args[0]);

		if (cmd == NULL)
		{
			fprintf(stderr, "%s: %d: %s: not found\n", av[0], line_count, args[0]);
			last_status = 127;
			line_count++;
			continue;
		}

		child_pid = fork();
		if (child_pid == 0)
		{
			execve(cmd, args, environ);
			exit(127);
		}
		else
		{
			wait(&status);
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}
		line_count++;
	}

	free(line);
	free_env_allocs();
	return (last_status);
}
