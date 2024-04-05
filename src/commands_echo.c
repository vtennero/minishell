/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:11:32 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/31 21:27:07 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	echo_wo_nl(t_shell *shell, char **args, int n_args, Command *cmd)
{
	int	i;

	i = 1;
	if (n_args == 1)
	{
		ft_printf("\n");
		return (0);
	}
	while (i < n_args)
	{
		if (cmd->fin == 0 && cmd->fout == 0 && ft_strcmp(args[i], "0") == 0)
			ft_printf("%s", shell_itoa(shell, shell->last_exit_status));
		else
		{
			ft_printf("%s", args[i]);
			if (i < n_args - 1)
				ft_printf(" ");
		}
		i++;
	}
	return (0);
}

static int	echo_w_nl(t_shell *shell, char **args, int n_args, Command *cmd)
{
	int	i;

	i = 0;
	while (i < n_args)
	{
		if (cmd->fin == 0 && cmd->fout == 0 && ft_strcmp(args[i], "0") == 0)
			ft_printf("%s", shell_itoa(shell, shell->last_exit_status));
		else
		{
			ft_printf("%s", args[i]);
			if (i < n_args - 1)
				ft_printf(" ");
		}
		i++;
	}
	ft_printf("\n");
	return (0);
}

int	builtin_echo(t_shell *shell, char **args, int n_args, Command *cmd)
{
	if (n_args == 0)
		ft_printf("\n");
	else if (ft_strcmp(args[0], "-n") == 0)
		echo_wo_nl(shell, args, n_args, cmd);
	else
		echo_w_nl(shell, args, n_args, cmd);
	return (0);
}
