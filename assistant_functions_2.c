#include "shell.h"

/**
 * *_strcpy - copies a string
 *
 * @dest: pointer to destionation string
 * @src: pointer to source string
 *
 * Return: a char string
 */

char *
_strcpy(char *dest, char *src)
{
	int i = 0;

	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = ('\0');
	return (dest);
}

/**
 * _calloc - Allocates memory for an array
 * @nmemb: Number of elements in array
 * @size: size of bytes
 * Return: Pointer to the allocated memory
 */

void *_calloc(unsigned int nmemb, unsigned int size)
{
	char *s;
	unsigned int i;

	if (nmemb == 0 || size == 0)
		return (NULL);

	s = malloc(nmemb * size);
	if (s == NULL)
		return (NULL);

	for (i = 0; i < (size * nmemb); i++)
		s[i] = 0;
	return (s);
}

/**
 * a_exit - Closes the program properly
 * @text: free memory
 * @i: total of commands
 * @cmd_line: 1st command
 * @exit_status: exit status
 * Return: Pointer to the allocated memory
 */

void a_exit(char **text, int i, char *cmd_line, int exit_status)
{
	int l;

	for (l = 0; l < i; l++)
	{
		free(text[l]);
	}
	free(text);
	free(cmd_line);
	exit(exit_status);
}

/**
 * _env - Prints the environment
 */

void _env(void)
{
	unsigned int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
	}
}

/**
 * signal_handler - Writes the prompt
 * @signal: Previous value of the signal
*/
void signal_handler(int signal __attribute__((unused)))
{
	write(STDOUT_FILENO, "\n$ ", 3);
}
