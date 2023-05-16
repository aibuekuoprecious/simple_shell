#include "shell.h"

/**
 * _eputs - writes a string to the stderr
 * @str: the string to be printed
 *
 * Return: void
 */
void _eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eputchar(str[i]);
		i++;
	}
}

/**
 * _eputchar - writes a single character to the stderr
 * @c: The character to print
 *
 * Return: On success, returns 1
 * On error, returns -1 and sets the variable errno, indicating the error
 */
int _eputchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];
  	ssize_t bytes_written;

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
			bytes_written = write(2, buf, i);
		if (bytes_written == -1)
		{
			/* handle error */
		}
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * _putfd - writes a character to a given fd
 * @c: The character to write
 * @fd: The filedescriptor to write to
 *
 * Return: On success, returns 1
 * On error, returns -1 and sets the variable errno, indicating the error
 */
int _putfd(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];
 	 ssize_t bytes_written;

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		bytes_written = write(fd, buf, i);
		if (bytes_written == -1)
		{
			/* handle errror */
		}
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * _putsfd - writes a string to the specified fd
 * @str: pointer to the string to be written
 * @fd: the filedescriptor to write to
 *
 * Return: the number of characters written
 */
int _putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _putfd(*str++, fd);
	}
	return (i);
}
