/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:10:53 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/29 10:10:11 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_cd_too_many_args(t_shell *shell)
{
	ft_putstr_fd(" bash: cd: too many arguments\n", 2);
	return (1);
	shell->last_exit_status = -255;
}

int	builtin_cd(t_shell *shell, char **args, int n_args)
{
	char	*path;

	if (n_args == 0)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			perror("cd: HOME not set\n");
			return (1);
		}
	}
	else if (n_args == 1)
	{
		path = args[0];
		if (chdir(path) != 0)
		{
			perror("cd");
			return (1);
		}
	}
	else
		return (builtin_cd_too_many_args(shell));
	return (0);
}
