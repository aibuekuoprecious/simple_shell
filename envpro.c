#include "shell.h"

/**
 * get_environ -  retrieves the environment variables
 * of the current process
 * @info: Structure containing potential arguments.
 * Used to maintain constant function prototype.
 *
 * Return: Always 0
 */
char **get_environ(data *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - removes an environment variable
 * from the process's environment
 * @info: Structure containing potential arguments.
 * Used to maintain constant function prototype.
 * @var: the string env var property
 *
 *  Return: 1 on delete, 0 otherwise
 */
int _unsetenv(data *info, char *var)
{
	linked_list *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = rm_node(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _setenv - sets a value for a given environment variable
 * in the process's environment
 * @info: Structure containing potential arguments.
 * Used to maintain constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *
 *  Return: Always 0
 */
int _setenv(data *info, char *var, char *value)
{
	char *buf = NULL;
	linked_list *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
