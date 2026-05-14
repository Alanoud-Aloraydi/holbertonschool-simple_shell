#include "shell.h"

static char *g_env_allocs[1024];
static int g_env_count;
static char **g_env_table;

/**
 * free_env_allocs - frees all tracked env allocations
 */
void free_env_allocs(void)
{
	int i;

	for (i = 0; i < g_env_count; i++)
		free(g_env_allocs[i]);

	free(g_env_table);
	g_env_count = 0;
	g_env_table = NULL;
}
/**
 * builtin_env - prints all environment variables
 */
void builtin_env(void)
{
	int i;
	for (i = 0; environ[i]; i++)
		printf("%s\n", environ[i]);
}

/**
 * get_env_value - gets an environment variable value
 * @name: variable name
 *
 * Return: value or NULL
 */
char *get_env_value(char *name)
{
	int i;
	size_t name_len;

	name_len = strlen(name);
	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], name, name_len) == 0 &&
		    environ[i][name_len] == '=')
			return (environ[i] + name_len + 1);
	}

	return (NULL);
}

/**
 * find_env - finds an environment variable
 * @name: variable name
 *
 * Return: index or -1
 */
static int find_env(char *name)
{
	int i;
	size_t name_len;

	name_len = strlen(name);
	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], name, name_len) == 0 &&
		    environ[i][name_len] == '=')
			return (i);
	}

	return (-1);
}

/**
 * add_env - adds a new environment variable
 * @new_entry: new entry
 */
static void add_env(char *new_entry)
{
	int i;
	char **new_table, **old_table;

	for (i = 0; environ[i]; i++)
		;

	new_table = malloc(sizeof(char *) * (i + 2));
	if (!new_table)
	{
		free(new_entry);
		return;
	}

	for (i = 0; environ[i]; i++)
		new_table[i] = environ[i];

	new_table[i] = new_entry;
	new_table[i + 1] = NULL;
	old_table = g_env_table;
	environ = new_table;
	g_env_table = new_table;
	free(old_table);

	g_env_allocs[g_env_count++] = new_entry;
}

/**
 * update_env - updates or adds an environment variable
 * @name: variable name
 * @value: new value
 */
void update_env(char *name, char *value)
{
	int i, j;
	char *new_entry;

	new_entry = malloc(strlen(name) + strlen(value) + 2);
	if (!new_entry)
		return;

	sprintf(new_entry, "%s=%s", name, value);
	i = find_env(name);
	if (i == -1)
	{
		add_env(new_entry);
		return;
	}

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
	g_env_allocs[g_env_count++] = new_entry;
}

