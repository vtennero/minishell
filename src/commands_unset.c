/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:15:00 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/23 22:18:12 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_shell *shell, char **args, int n_args)
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
	return 0;
}
