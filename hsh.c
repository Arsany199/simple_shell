#include "shell.h"

/**
 * main - Prints the prompt and calls
 * @ac: argument counter
 * @av: argument vector
 * Return: Always 0
 */

int main(__attribute__((unused)) int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	int i = 1, count = 0;

	while (i != -1)
	{
		count++;
		signal(SIGINT, signal_handler);
		if (isatty(STDIN_FILENO) == 1)
			write(STDOUT_FILENO, "$ ", 2);
		i = getline(&line, &len, stdin);
		if (i < 0)
		{
			free(line);
			exit(0);
		}
		str_to_array(line, count, av);
		free(line);
		line = NULL;
	}
	return (0);
}

/**
 * str_to_array - Takes a string and turns it into a list of strings
 * @cmd_line: string
 * @count: line counter
 * @argv: argument vector
 * Return: A list of strings
 */

int str_to_array(char *cmd_line, int count, char **argv)
{
	char **token_array, *token, *tmp = 0, *exit = {"exit"}, *envi = {"env"};
	int i = 0, j = 0, k = 0, l = 0, exit_status = 0;

	tmp = _strdup(cmd_line);
	token = strtok(tmp, strtok_delim);
	while (token != NULL)
		token = strtok(NULL, strtok_delim), i++;
	free(tmp);
	if (i != 0)
	{
		token_array = _calloc((i + 1), (sizeof(char *)));
		if (token_array == NULL)
			return ('\0');
		token = strtok(cmd_line, strtok_delim);
		while (token != NULL)
		{
			token_array[j] = _calloc((_strlen(token) + 1), sizeof(char));
			if (token_array[j] == NULL)
			{
				while (k < j)
					free(token_array[k]), k++;
				free(token_array);
			}
			_strncpy(token_array[j], token, _strlen(token) + 1);
			token = strtok(NULL, strtok_delim), j++;
		}
		token_array[j] = NULL;
		if (_strcmp(token_array[0], envi) == 0 ||
		 _strcmp(token_array[0], "printenv") == 0)
			_env();
		if (_strcmp(token_array[0], exit) == 0)
			a_exit(token_array, i, cmd_line, exit_status);
		exit_status = _exec(token_array, i, cmd_line, count, argv);
		while (l < i)
			free(token_array[l]), l++;
		free(token_array);
	}
	return (exit_status);
}

/**
 * _exec - Creates a child processes
 * @cmd_list: Array of strings
 * for the programm to execute
 * @i: number of arguments
 * @count: line counter
 * @argv: argument vector
 * @cmd_line: Command line
 * Return: 0 if success, 1 if failed
 */

int _exec(char **cmd_list, int i, char *cmd_line, int count, char **argv)
{
	pid_t childpid;
	int status, exit_status = 0;
	struct stat st;
	char *directory, *not_command = {"it isn't a command"};

	switch (childpid = fork())
	{
	case -1:
		perror("fork error");
		return (1);
	case 0:
		if (stat(cmd_list[0], &st) == 0 && st.st_mode & S_IXUSR)
		{
			if (execve(cmd_list[0], cmd_list, environ) == -1)
				perror("$ Error"), exit(exit_status);
			else
				exit(EXIT_SUCCESS);
		}
		else
		{
			directory = _path(cmd_list[0]);
			if (_strcmp(directory, not_command) == 0)
			{
				command_not_found(i, cmd_list, count, argv), free(cmd_line);
				exit(EXIT_FAILURE);
			}
			else
			{
				if (execve(directory, cmd_list, environ) == -1)
					free(directory), perror("$ Error"), exit(exit_status);
				else
					exit(EXIT_SUCCESS);
			}
		}
	default:
		wait(&status);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	return (exit_status);
}

/**
 * _path - Returns all the directories
 * @command: The first argument
 * Return: Directories
 */

char *_path(char *command)
{
	int i;
	char var[] = "PATH", *path, *token, *env, *dir_temp;

	for (i = 0; environ[i]; i++)
	{
		env = _strdup(environ[i]);
		token = strtok(env, "=");
		if (_strcmp(token, var) == 0)
		{
			token = strtok(NULL, "=");
			dir_temp = _strdup(token);
			path = directory(dir_temp, command);
			free(dir_temp);
		}
		free(env);
	}
	return (path);
}

/**
 * directory - Checks if the command typed by User
 * @temporal_dir: string
 * @command: First command
 * Return: The path if the command exists
 */

char *directory(char *temporal_dir, char *command)
{
	char *path, *token, slash[] = {'/'}, flag = 0;
	char *not_command = {"it isn't a command"};
	struct stat st;

	token = strtok(temporal_dir, ":");
	while (token != NULL)
	{
		path = _calloc(_strlen(token) + _strlen(command) + 2, sizeof(char));
		_strcpy(path, token);
		_strcat(path, slash);
		_strcat(path, command);
		if (stat(path, &st) == 0 && st.st_mode & S_IXUSR)
		{
			flag++;
			break;
		}
		token = strtok(NULL, ":");
		free(path);
	}
	if (flag == 1)
		return (path);
	else
		return (not_command);
}
