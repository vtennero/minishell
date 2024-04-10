/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:11:36 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/19 22:33:49 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token_node *head)
{
	t_token_node	*current;

	current = head;
	ft_printf("\n");
	while (current != NULL)
	{
		ft_printf("t_token: %s \t | Type: %d\n", current->token.value,
			current->token.type);
		current = current->next;
	}
	ft_printf("*********************************\n");
}

void	print_command(const t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	ft_printf("t_cmd: %s\n", cmd->name);
	ft_printf("Arguments: ");
	while (i < cmd->arg_count)
		ft_printf("|%s| ", cmd->args[i++]);
	ft_printf("\n");
	ft_printf("Fin is: %d\n", cmd->fin);
	ft_printf("Fout is: %d\n", cmd->fout);
	if (cmd->redirect_in)
		ft_printf("Input redirected from: %s\n", cmd->redirect_in);
	if (cmd->redirect_out)
		ft_printf("Output redirected to: %s\n", cmd->redirect_out);
	if (cmd->redirect_append)
		ft_printf("Output appended to: %s\n", cmd->redirect_append);
	if (cmd->heredoc_delimiter)
		ft_printf("Heredoc delimiter: %s\n", cmd->heredoc_delimiter);
	if (cmd->heredoc_temp_path)
		ft_printf("Heredoc temporary file path: %s\n", cmd->heredoc_temp_path);
	if (cmd->next)
		ft_printf("Piped to next command\n");
}

void	print_command_table(const t_cmd_table *table)
{
	t_cmd	*current;

	if (!table || !table->head)
	{
		ft_printf("t_cmd table is empty.\n");
		return ;
	}
	ft_printf("\nt_cmd Table (Count: %d):\n", table->command_count);
	current = table->head;
	while (current)
	{
		print_command(current);
		current = current->next;
		if (current)
		{
			ft_printf("----------\n");
		}
	}
	ft_printf("*********************************\n\n");
}

int	is_token_type_present(t_token_node *head, int type)
{
	t_token_node	*current;

	current = head;
	while (current != NULL)
	{
		if (current->token.type == type)
			return (1);
		current = current->next;
	}
	return (0);
}

int	get_token_list_length(t_token_node *head)
{
	int			length;
	t_token_node	*current;

	length = 0;
	current = head;
	while (current != NULL)
	{
		length++;
		current = current->next;
	}
	return (length);
}
