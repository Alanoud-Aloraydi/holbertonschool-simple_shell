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
	int i;

	if (!command)
		return (NULL);

	path_env = NULL;
	i = 0;

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path_env = environ[i] + 5;
			break;
		}
		i++;
	}

	if (!path_env || path_env[0] == '\0')
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	token = strtok(path_copy, ":");

	while (token)
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
