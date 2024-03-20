/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:15:00 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/20 16:28:42 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_unset(t_shell *shell, char **args, int n_args)
{
	int	i;
	int	nchar;

	i = 0;
	while (i < n_args)
	{
		nchar = find_index_char(args[i], '=');
		remove_var(shell, args[i], nchar);
		i++;
	}
}
