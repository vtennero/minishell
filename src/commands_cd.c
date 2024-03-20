/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:10:53 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/20 16:20:22 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_cd(t_shell *shell, char **args, int n_args)
{
	char	*path;

	if (n_args == 0)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			perror("cd: HOME not set\n");
			return ;
		}
	}
	else if (n_args == 1)
	{
		path = args[0];
		if (chdir(path) != 0)
			perror("cd");
		shell->last_exit_status = 1;
	}
	else
	{
		perror("bash: cd: too many arguments\n");
		shell->last_exit_status = 1;
	}
}
