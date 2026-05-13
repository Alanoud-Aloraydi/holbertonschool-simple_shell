#include "shell.h"

/**
 * get_path - finds command in PATH
 * @command: command entered
 *
 * Return: full path or NULL
 */
char *get_path(char *command)
{
	char *path_env, *path_copy, *token;
	static char full_path[1024];

	if (access(command, X_OK) == 0)
		return (command);

	path_env = getenv("PATH");

	if (path_env == NULL)
		return (NULL);

	path_copy = strdup(path_env);

	token = strtok(path_copy, ":");

	while (token != NULL)
	{
		sprintf(full_path, "%s/%s", token, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		token = strtok(NULL, ":");
	}

	free(path_copy);

	return (NULL);
}
