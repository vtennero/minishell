/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:11:37 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/23 22:18:25 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(t_shell *shell, char **args, int n_args)
{
	int	i;

	i = 0;
	if (n_args == 0)
		print_export(shell, 1);
	else
	{
		while (i < n_args)
		{
			process_env_arg(shell, args[i]);
			i++;
		}
	}
	return 0;
}
