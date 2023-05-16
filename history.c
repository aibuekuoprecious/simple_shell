#include "shell.h"

/**
 * get_history_file -  returns the path to the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_history_file(data *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HISTORY_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HISTORY_FILE);
	return (buf);
}

/**
 * write_history - writes the current contents
 * of the history list to the history file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(data *info)
{
	ssize_t fd;
	char *filename = get_history_file(info);
	linked_list *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - reads the contents of the history file
 * into the history list
 * @info: the parameter struct
 *
 * Return: history_num on success, 0 otherwise
 */
int read_history(data *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_history(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_history(info, buf + last, linecount++);
	free(buf);
	info->history_num = linecount;
	while (info->history_num-- >= MAX_HISTORY)
		rm_node(&(info->history), 0);
	renumber_history(info);
	return (info->history_num);
}

/**
 * build_history - adds a new command to the history list
 * and increments the history_num counter.
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, history_num
 *
 * Return: Always 0
 */
int build_history(data *info, char *buf, int linecount)
{
	linked_list *node = NULL;

	if (info->history)
		node = info->history;
	add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * renumber_history - renumbers the history list starting from 1.
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new history_num
 */
int renumber_history(data *info)
{
	linked_list *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->history_num = i);
}
