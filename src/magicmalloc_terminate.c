/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magicmalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:42:10 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/23 14:26:28 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_free(t_shell *shell, void *ptr)
{
	MemNode	**current;
	MemNode	*entry;

	current = &shell->mem_tracker.head;
	while (*current)
	{
		entry = *current;
		if (entry->ptr == ptr)
		{
			*current = entry->next;
			free(entry->ptr);
			free(entry);
			return ;
		}
		current = &entry->next;
	}
}

void	shell_cleanup(t_shell *shell)
{
	MemNode	*current;
	MemNode	*next;

	current = shell->mem_tracker.head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		current->ptr = NULL;
		free(current);
		current = NULL;
		current = next;
	}
	shell->mem_tracker.head = NULL;
}

void	shexit(t_shell *shell, int exit_code)
{
	int	exit_s;

	exit_s = shell->last_exit_status;
	shell_cleanup(shell);
	free(shell);
	shell = NULL;
	exit(exit_s);
	(void)exit_code;
}
