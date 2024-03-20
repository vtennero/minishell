/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:11:37 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/20 16:27:16 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_export(t_shell *shell, char **args, int n_args)
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
}
