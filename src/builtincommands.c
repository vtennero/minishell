/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtincommands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:10:58 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/16 14:35:03 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void builtin_cd(char* path)
{
	ft_printf("builtin_cd\n");
	if (chdir(path) == -1) {
		perror("cd");
	}
	ft_printf("Current directory: %s\n", path);
	ft_printf("Changing directory to: %s\n", path);
}

// Prints the current working directory to stdout.
void builtin_pwd(void)
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		ft_printf("%s\n", cwd);
	} else {
		perror("getcwd");
	}
}

// Prints the given arguments to stdout, handling the `-n` option to not output the trailing newline.
void builtin_echo(char** args, int n_args)
{
	int i = 0;
	int newline = 1;
	// ft_printf("builtin_echo n_args %d\n", n_args);
	// ft_printf("%s\n", args[0]);
	if (n_args > 1 && strcmp(args[0], "-n") == 0)
	{
		// ft_printf("found -n\n");
		newline = 0;
		i++;
	}
	while (i < n_args)
	{
		ft_printf("%s", args[i]);
		if (i < n_args - 1)
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
}

// Sets or exports an environment variable with the given name to the given value.
void builtin_export(char* variable, char* value)
{
	if (setenv(variable, value, 1) == -1) {
		perror("export");
	}
}

// Removes the environment variable with the given name from the shell's environment.
void builtin_unset(char* variable)
{
	if (unsetenv(variable) == -1) {
		perror("unset");
	}
}

// Lists all environment variables to stdout.
void builtin_env(void)
{
	extern char** environ;
	for (char** env = environ; *env != NULL; env++)
		ft_printf("%s\n", *environ);
}

// Exits the shell.
void builtin_exit(void)
{
	ft_printf("exit\n");
	exit(0);
}
