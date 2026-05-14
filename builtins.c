#include "shell.h"

/**
 * cd_target - gets cd target directory
 * @args: argument array
 * @av0: program name
 * @line_count: current line number
 * @is_dash: cd dash flag
 *
 * Return: directory or NULL
 */
static char *cd_target(char **args, char *av0, int line_count, int *is_dash)
{
	char *dir;

	*is_dash = 0;
	if (args[1] == NULL)
	{
		dir = get_env_value("HOME");
		if (!dir)
			fprintf(stderr, "%s: %d: cd: HOME not set\n",
				av0, line_count);
		return (dir);
	}

	if (strcmp(args[1], "-") == 0)
	{
		dir = get_env_value("OLDPWD");
		if (!dir)
			fprintf(stderr, "%s: %d: cd: OLDPWD not set\n",
				av0, line_count);
		*is_dash = 1;
		return (dir);
	}

	return (args[1]);
}

/**
 * finish_cd - updates PWD and OLDPWD
 * @oldpwd: previous working directory
 * @is_dash: cd dash flag
 */
static void finish_cd(char *oldpwd, int is_dash)
{
	char cwd[1024];

	update_env("OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		update_env("PWD", cwd);
		if (is_dash)
			printf("%s\n", cwd);
	}
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
	char *dir, oldpwd[1024];
	int is_dash;

	if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
		return (1);

	/* Changed: handles HOME and OLDPWD through cd_target */
	dir = cd_target(args, av0, line_count, &is_dash);
	if (!dir)
		return (1);

	if (chdir(dir) == -1)
	{
		fprintf(stderr, "%s: %d: cd: can't cd to %s\n",
			av0, line_count, dir);
		return (1);
	}

	/* Changed: updates PWD and OLDPWD after successful cd */
	finish_cd(oldpwd, is_dash);
	return (0);
}

