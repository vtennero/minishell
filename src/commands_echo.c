/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:11:32 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/27 23:16:14 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(t_shell *shell, char **args, int n_args,Command* cmd)
{
	int	i;
	int	newline;

	i = 0;
	newline = 1;

	if (n_args > 1 && strcmp(args[0], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (i < n_args)
	{
		if (cmd->fin==0 && cmd->fout==0 && ft_strcmp(args[i],"0")==0)
		{
			ft_printf("%s",ft_itoa(shell->last_exit_status));
		}
		else
		{
			ft_printf("%s", args[i]);
			if (i < n_args - 1)
				ft_printf(" ");
		}
		i++;
	}
	if (newline)
		ft_printf("\n");
	(void)shell;
	return 0;
}
