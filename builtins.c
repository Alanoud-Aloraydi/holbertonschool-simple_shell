#include "shell.h"

static char *g_env_allocs[10];
static int g_env_count;

/**
 * free_env_allocs - frees all tracked env allocations
 */
void free_env_allocs(void)
{
	int i;

	for (i = 0; i < g_env_count; i++)
		free(g_env_allocs[i]);
	g_env_count = 0;
}

/**
 * update_env - updates an environment variable
 * @name: variable name
 * @value: new value
 */
void update_env(char *name, char *value)
{
	int i, j;
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
			for (j = 0; j < g_env_count; j++)
			{
				if (g_env_allocs[j] == environ[i])
				{
					free(g_env_allocs[j]);
					g_env_allocs[j] = new_entry;
					environ[i] = new_entry;
					return;
				}
			}
			environ[i] = new_entry;
			if (g_env_count < 10)
				g_env_allocs[g_env_count++] = new_entry;
			return;
		}
	}
	free(new_entry);
}

/**
 * builtin_cd - changes current directory
 * @args: argument array
 * @av0: program name
 * @line_count: current line number
 *
 * Return: 0 on success, 1 on failure
 */
int builtin_cd(char **args, char *av0, int line_count)
{
	char *dir, cwd[1024], oldpwd[1024];
	int i, is_dash;

	is_dash = 0;
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
			fprintf(stderr, "%s: %d: cd: HOME not set\n",
				av0, line_count);
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
			fprintf(stderr, "%s: %d: cd: OLDPWD not set\n",
				av0, line_count);
			return (1);
		}
		is_dash = 1;
	}
	else
		dir = args[1];
	if (chdir(dir) == -1)
	{
		fprintf(stderr, "%s: %d: cd: can't cd to %s\n",
			av0, line_count, dir);
		return (1);
	}
	update_env("OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		update_env("PWD", cwd);
		if (is_dash)
			printf("%s\n", cwd);
	}
	return (0);
}
