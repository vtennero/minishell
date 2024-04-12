/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:39:58 by cliew             #+#    #+#             */
/*   Updated: 2024/04/13 07:45:22 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_command_entry(t_shell *shell, char *name)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)shell_malloc(shell, sizeof(t_cmd));
	if (!cmd)
	{
		ft_putstr_fd("Failed to allocate t_cmd", 2);
		exit(EXIT_FAILURE);
	}
	if (cmd->name!=NULL)
		cmd->name = shell_strdup(shell, name);
	else
		cmd->name=NULL;
	cmd->fin = 0;
	cmd->fout = 0;
	cmd->arg_count = 0;
	return (cmd);
}

void	add_argument(t_shell *shell, t_cmd *cmd, char *arg)
{
	cmd->args[cmd->arg_count++] = shell_strdup(shell, arg);
	cmd->args[cmd->arg_count] = NULL;
}

t_cmd	*create_command_set(t_shell *shell, t_token_node *node)
{
	t_cmd			*current_command;
	t_token_node	*temp;
	int				argc;

	current_command = NULL;
	temp = node;
	argc = 0;
	while (temp && temp != NULL && temp->token.type != TOKEN_PIPE)
	{
		if (temp->token.type == TOKEN_COMMAND || temp->token.type == 13)
		{
			current_command = create_command_entry(shell, temp->token.value);
			while (temp->next && temp->next->token.type == TOKEN_ARG)
			{
				argc++;
				temp = temp->next;
			}
		}
		else if (temp->next && temp->next->token.type == TOKEN_PIPE && current_command==NULL)
		{
			current_command = create_command_entry(shell,NULL);
			while (temp->next && temp->next->token.type == TOKEN_ARG)
			{
				argc++;
				temp = temp->next;
			}
		}
		temp = temp->next;
	}
	if (!current_command){
		ft_putstr_fd("syntax error near unexpected token `newline'", 2);
		return NULL;

	}
		
	current_command->args = (char **)shell_malloc(shell, (argc + 5)
			* sizeof(char *));
	return (current_command);
}

void	handle_token(t_shell *shell, t_token_node *ct, t_cmd *cc)
{
	if (ct->token.type == TOKEN_REDIR_IN && ct->next)
	{
		if (ct->next != NULL && ct->next->token.type == 1)
			set_rin(shell, cc, ct->next->token.value);
		*ct = *ct->next;
	}
	else if ((ct->token.type == 3 || ct->token.type == 4) && ct->next)
	{
		if (ct->next != NULL && ct->next->token.type == 1)
			set_rout(shell, cc, ct->next->token.value, ct->token.type == 4);
		*ct = *ct->next;
	}
	else if (ct->token.type == 5)
	{
		if (ct->next != NULL && ct->next->token.type == 1)
			cc->heredoc_delimiter = shell_strdup(shell, ct->next->token.value);
		*ct = *ct->next;
	}
	else if (ct->token.type == 1)
		add_argument(shell, cc, ct->token.value);
	else if (((ct->token.type == 2) || (ct->token.type == 3)
			|| (ct->token.type == 4) || (ct->token.type == 5)) && !ct->next)
		ft_puterr("syntax error near unexpected token `newline'", 2);
}

void	pipe_modify_fin_fout(t_token_node *current_token, t_cmd *cc,
		int *pipe_exist)
{
	if (*pipe_exist == 1 && current_token->token.type != TOKEN_PIPE && cc->name!=NULL)
	{
		if (ft_strcmp(cc->name, "ls") != 0 && ft_strcmp(cc->name, "echo") != 0)
			if (cc->fin != -1)
				cc->fin = -99;
		*pipe_exist = 0;
	}
	if (current_token->next && current_token->next->token.type == TOKEN_PIPE && cc->name!=NULL)  
	{
		if (cc->fout == 0)
			cc->fout = -99;
		*pipe_exist = 1;
	}
}
