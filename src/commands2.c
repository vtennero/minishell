/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:40:14 by cliew             #+#    #+#             */
/*   Updated: 2024/04/10 18:46:00 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_head(t_cmd *current_command, t_cmd_table *table)
{
	t_cmd	*last_command;

	if (table->head == NULL)
		table->head = current_command;
	else
	{
		last_command = table->head;
		while (last_command->next != NULL)
		{
			last_command = last_command->next;
		}
		last_command->next = current_command;
	}
	table->command_count++;
}

t_cmd	*process_command_set(t_shell *shell, t_cmd_table *table,
		t_token_node **ct, int *pipe_exist)
{
	t_cmd	*cc;

	cc = NULL;
	if (*pipe_exist == 1 || table->head == NULL)
		cc = create_command_set(shell, *ct);
	while (*ct != NULL && (*ct)->token.type != TOKEN_PIPE)
	{
		handle_token(shell, *ct, cc);
		pipe_modify_fin_fout(*ct, cc, pipe_exist);
		*ct = (*ct)->next;
	}
	return (cc);
}

void	update_token_node(t_token_node **ct)
{
	if (*ct != NULL)
		*ct = (*ct)->next;
}

void	process_token_nodes(t_shell *shell, t_cmd_table *table,
		t_token_node *tokens)
{
	int				pipe_exist;
	t_token_node	*ct;
	t_cmd			*cc;

	pipe_exist = 0;
	ct = tokens;
	while (ct != NULL)
	{
		cc = process_command_set(shell, table, &ct, &pipe_exist);
		update_token_node(&ct);
		update_head(cc, table);
	}
}

t_cmd_table	*create_command_table(t_shell *shell, t_token_node *tokens)
{
	t_cmd_table	*table;

	table = initialize_command_table(shell);
	if (tokens == NULL)
		return (table);
	process_token_nodes(shell, table, tokens);
	if (!table || !table->head)
		prepare_heredocs_in_command_table(table);
	return (table);
}
