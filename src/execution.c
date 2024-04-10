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

void	execute_command_table(t_shell *shell, CommandTable *table)
{
	Command	*current;

	if (table)
		shell->table = table;
	current = table->head;
	(void)shell;
	pipex(current, shell);
	cleanup_heredocs_in_command_table(table);
}
