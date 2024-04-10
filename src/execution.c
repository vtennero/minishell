/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:56:56 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 10:56:56 by vitenner         ###   ########.fr       */
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
