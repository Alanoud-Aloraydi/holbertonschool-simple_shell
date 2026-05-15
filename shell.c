#include "shell.h"

/**
 * handle_ctrl_c - Handles Ctrl+C signal safely.
 * @sig: Signal number.
 */
void handle_ctrl_c(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * main - UNIX command line interpreter that handles arguments cleanly.
 * @ac: Argument count.
 * @av: Argument vector.
 *
 * Return: Last execution status code.
 */
int main(int ac, char **av)
{
	char *line = NULL, *token, *cmd;
	size_t len = 0;
	ssize_t nread;
	char *args[1024];
	int i, status, last_status = 0, line_count = 1;
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
			if (args[1] != NULL)
				last_status = atoi(args[1]);
			free(line);
			free_env_allocs();
			exit(last_status);
		}
		if (strcmp(args[0], "env") == 0)
		{
			builtin_env();
			line_count++;
			continue;
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
			fprintf(stderr, "%s: %d: %s: not found\n",
				av[0], line_count, args[0]);
			last_status = 127;
			line_count++;
			continue;
		}
		child_pid = fork();
		if (child_pid == 0)
		{
			if (execve(cmd, args, environ) == -1)
			{
				if (cmd != args[0])
					free(cmd);
				free(line);
				free_env_allocs();
				exit(127);
			}
		}
		else
		{
			wait(&status);
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			if (cmd != args[0])
				free(cmd);
		}
		line_count++;
	}
	free(line);
	free_env_allocs();
	return (last_status);
}
