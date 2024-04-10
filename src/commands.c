/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:56:33 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 10:56:33 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

CommandTable	*initialize_command_table(t_shell *shell)
{
	CommandTable	*table;

	table = (CommandTable *)shell_malloc(shell, sizeof(CommandTable));
	if (!table)
	{
		ft_putstr_fd("Failed to allocate CommandTable", 2);
		exit(EXIT_FAILURE);
	}
	table->head = NULL;
	table->command_count = 0;
	return (table);
}

void	set_rin(t_shell *shell, Command *cmd, char *filename)
{
	int	fd;

	cmd->redirect_in = shell_strdup(shell, filename);
	fd = open(filename, O_RDWR);
	if (cmd->fin != -1)
		cmd->fin = fd;
}

void	set_rout(t_shell *shell, Command *cmd, char *filename, int append)
{
	int	fd;

	if (!cmd || !filename)
		return ;
	fd = 0;
	if (cmd->fout != -1)
	{
		if (cmd->fin == -1)
		{
			return ;
		}
		if (append)
		{
			cmd->redirect_out = shell_strdup(shell, filename);
			fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0666);
		}
		else
		{
			cmd->redirect_out = shell_strdup(shell, filename);
			fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
		}
		cmd->fout = fd;
	}
}

void	free_command_table(CommandTable *table)
{
	Command	*cmd;
	Command	*next_cmd;
	int		i;

	if (!table)
		return ;
	i = 0;
	cmd = table->head;
	while (cmd)
	{
		next_cmd = cmd->next;
		free(cmd->name);
		while (i < cmd->arg_count)
			free(cmd->args[i++]);
		free(cmd->args);
		free(cmd->redirect_in);
		free(cmd->redirect_out);
		free(cmd->redirect_append);
		free(cmd);
		cmd = next_cmd;
	}
	free(table);
}

Command	*create_command_entry(t_shell *shell, char *name)
{
	Command	*cmd;

	cmd = (Command *)shell_malloc(shell, sizeof(Command));
	if (!cmd)
	{
		ft_putstr_fd("Failed to allocate Command", 2);
		exit(EXIT_FAILURE);
	}
	cmd->name = shell_strdup(shell, name);
	cmd->type = CMD_EXTERNAL;
	cmd->fin = 0;
	cmd->fout = 0;
	return (cmd);
}

void	add_argument(t_shell *shell, Command *cmd, char *arg)
{
	cmd->args[cmd->arg_count++] = shell_strdup(shell, arg);
	cmd->args[cmd->arg_count] = NULL;
}

Command	*create_command_set(t_shell *shell, TokenNode *node)
{
	Command		*current_command;
	TokenNode	*temp;
	int			argc;

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
		temp = temp->next;
	}
	if (!current_command)
		ft_puterr("syntax error near unexpected token `newline'", 2);
	current_command->args = (char **)shell_malloc(shell, (argc + 2)
			* sizeof(char *));
	return (current_command);
}

void	handle_token(t_shell *shell, TokenNode *ct, Command *cc)
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

void	pipe_modify_fin_fout(TokenNode *current_token, Command *current_command,
		int *pipe_exist)
{
	if (*pipe_exist == 1 && current_token->token.type != TOKEN_PIPE)
	{
		if (ft_strcmp(current_command->name, "ls") != 0
			&& ft_strcmp(current_command->name, "echo") != 0)
			if (current_command->fin != -1)
				current_command->fin = -99;
		*pipe_exist = 0;
	}
	if (current_token->next && current_token->next->token.type == TOKEN_PIPE)
	{
		if (current_command->fout == 0)
			current_command->fout = -99;
		*pipe_exist = 1;
	}
}

void	update_head(Command *current_command, CommandTable *table)
{
	Command	*last_command;

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

CommandTable	*create_command_table(t_shell *shell, TokenNode *tokens)
{
	CommandTable	*table;
	TokenNode		*ct;
	Command			*cc;
	int				pipe_exist;

	table = initialize_command_table(shell);
	ct = tokens;
	if (ct==NULL)
		return table;
	pipe_exist = 0;
	cc = NULL;
	while (ct != NULL)
	{
		if (pipe_exist == 1 || table->head == NULL)
			cc = create_command_set(shell, ct);
		while (ct != NULL && ct->token.type != TOKEN_PIPE)
		{
			handle_token(shell, ct, cc);
			pipe_modify_fin_fout(ct, cc, &pipe_exist);
			ct = ct->next;
		}
		if (ct != NULL)
			ct = ct->next;
		update_head(cc, table);
	}
	if (!table || !table->head)
		prepare_heredocs_in_command_table(table);
	return (table);
}
