#include "shell.h"

/**
 * handle_builtins - Matches and dispatches shell built-in commands.
 * @args: Tokenized argument vectors.
 * @line: The raw buffer from getline to free upon exit.
 * @av0: The program execution name (argv[0]) for cd error routing.
 * @line_count: The shell loop execution counter.
 *
 * Return: 1 if a built-in was matched and handled, 0 otherwise.
 */
int handle_builtins(char **args, char *line, char *av0, int line_count)
{
	if (args[0] == NULL)
		return (0);


	if (strcmp(args[0], "exit") == 0)
	{
		free(line);
		free_env_allocs(); 
		exit(EXIT_SUCCESS);
	}

	if (strcmp(args[0], "env") == 0)
	{
		builtin_env();
		return (1);
	}


	if (strcmp(args[0], "cd") == 0)
	{
		builtin_cd(args, av0, line_count);
		return (1);
	}

	return (0);
}
