/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 17:38:02 by cliew             #+#    #+#             */
/*   Updated: 2024/04/10 17:38:02 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command_table(t_shell *shell, t_cmd_table *table)
{
	t_cmd	*current;

	if (table && table->head)
	{
		shell->table = table;
		current = table->head;
		pipex(current, shell);
		cleanup_heredocs_in_command_table(table);
	}
}

void	kill_children(t_shell *shell)
{
	int	i;

	i = 0;
	shell->token_head = NULL;
	while (i < shell->child_num)
	{
		if (kill(shell->child_pid[i], 0) == 0)
		{
			kill(shell->child_pid[i], SIGTERM);
		}
		i++;
	}
}
