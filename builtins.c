#include "shell.h"

/**
 * update_env - updates an environment variable
 * @name: variable name
 * @value: new value
 */
void update_env(char *name, char *value)
{
	int i;
	size_t name_len;
	char *new_entry;

	name_len = strlen(name);
	new_entry = malloc(name_len + strlen(value) + 2);
	if (!new_entry)
		return;
	sprintf(new_entry, "%s=%s", name, value);
	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], name, name_len) == 0 &&
			environ[i][name_len] == '=')
		{
			environ[i] = new_entry;
			return;
		}
	}
	free(new_entry);
}

/**
 * builtin_cd - changes current directory
 * @args: argument array
 * @av0: program name
 *
 * Return: 0 on success, 1 on failure
 */
int builtin_cd(char **args, char *av0)
{
	char *dir, cwd[1024], oldpwd[1024];
	int i;

	if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
		return (1);
	if (args[1] == NULL)
	{
		dir = NULL;
		for (i = 0; environ[i]; i++)
		{
			if (strncmp(environ[i], "HOME=", 5) == 0)
			{
				dir = environ[i] + 5;
				break;
			}
		}
		if (!dir)
		{
			fprintf(stderr, "%s: cd: HOME not set\n", av0);
			return (1);
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		dir = NULL;
		for (i = 0; environ[i]; i++)
		{
			if (strncmp(environ[i], "OLDPWD=", 7) == 0)
			{
				dir = environ[i] + 7;
				break;
			}
		}
		if (!dir)
		{
			fprintf(stderr, "%s: cd: OLDPWD not set\n", av0);
			return (1);
		}
	}
	else
		dir = args[1];
	if (chdir(dir) == -1)
	{
		fprintf(stderr, "%s: cd: %s: No such file or directory\n",
			av0, dir);
		return (1);
	}
	update_env("OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		update_env("PWD", cwd);
	return (0);
}
