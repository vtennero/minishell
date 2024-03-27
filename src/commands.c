/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:01:04 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/26 11:50:51 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static TokenHandler tokenHandlers[] = {
	handle_command_token, // Assuming 0 corresponds to COMMAND
	handle_arg_token,     // Assuming 1 corresponds to ARG
	handle_redirect_in_token, // etc.
	handle_redirect_out_token,
	// Populate other handlers according to the token type values...
	handle_debug_weird_arg,
	handle_debug_weird_arg,
	handle_debug_weird_arg,
	handle_debug_weird_arg,
	handle_debug_weird_arg,
	handle_debug_weird_arg,
	handle_debug_weird_arg,
	handle_debug_weird_arg,
	handle_debug_weird_arg,
};

CommandTable* initialize_command_table(t_shell *shell)
{
	CommandTable	*table;
	
	table = (CommandTable *)shell_malloc(shell, sizeof(CommandTable));
	if (!table)
	{
		perror("Failed to allocate CommandTable");
		exit(EXIT_FAILURE);
	}
	table->head = NULL;
	table->command_count = 0;
	return table;
}

void set_redirect_in(t_shell *shell, Command* cmd, char* filename) {
	cmd->redirect_in = shell_strdup(shell, filename);
	int fd = open(filename, O_RDONLY);
	// if (fd ==-1)
	//     ft_puterr(ft_strjoin_nconst(filename, " : File not exists/permission error" ), 1);
	cmd->fin = fd;
}

void set_redirect_out(t_shell *shell, Command* cmd, char* filename, int append) {
	if (!cmd || !filename) return;
	int fd = 0;

	// Free any existing redirection target to avoid memory leaks
	// if (cmd->redirect_out) {
	//     free(cmd->redirect_out);
	//     cmd->redirect_out = NULL;
	// }
	
	// if (cmd->redirect_append) {
	//     free(cmd->redirect_append);
	//     cmd->redirect_append = NULL;
	// }

	if (append) {
		cmd->redirect_out = shell_strdup(shell, filename);
		fd = open(filename, O_RDWR | O_CREAT | O_APPEND,0666);
	} else {

		cmd->redirect_out = shell_strdup(shell, filename);
		fd = open(filename, O_RDWR | O_CREAT,0666);
	}
	// if (fd ==-1)
	//     ft_puterr(ft_strjoin_nconst(filename, "File not exists/permission error" ), 1);
	cmd->fout = fd;
}

Command		*create_command_entry(t_shell *shell, char* name)
{
	Command	*cmd;
	cmd = (Command *)shell_malloc(shell, sizeof(Command));
	if (!cmd) {
		perror("Failed to allocate Command");
		exit(EXIT_FAILURE);
	}
	cmd->name = shell_strdup(shell, name);
	cmd->fin=0;
	cmd->fout=0;
	return (cmd);
}

void	add_argument(t_shell *shell, Command* cmd, char* arg)
{
	cmd->args[cmd->arg_count++] = shell_strdup(shell, arg);
	cmd->args[cmd->arg_count] = NULL;
}

void	handle_command_token(t_shell* shell, CommandTable* table, TokenNode** current_token, Command** last_command)
{
	Command		*current_command;
	int			arg_count;
	TokenNode	*temp;

	current_command = create_command_entry(shell, (*current_token)->token.value);
	arg_count = 0;
	temp = (*current_token)->next;
	while (temp && temp->token.type == TOKEN_ARG)
	{
		arg_count++;
		temp = temp->next;
	}
	current_command->args = (char**)shell_malloc(shell, (arg_count + 1) * sizeof(char*));
	if (table->head == NULL)
		table->head = current_command;
	else if (*last_command)
		(*last_command)->next = current_command;
	*last_command = current_command;
	table->command_count++;
	print_command_table(table);
}

void handle_arg_token(t_shell* shell, CommandTable* table, TokenNode** current_token, Command** last_command)
{
	if (*last_command != NULL)
		add_argument(shell, *last_command, (*current_token)->token.value);
	(void)table;
}

void handle_redirect_in_token(t_shell* shell, CommandTable* table, TokenNode** current_token, Command** last_command) {
	if ((*current_token)->next != NULL && (*current_token)->next->token.type == TOKEN_ARG)
	{
		set_redirect_in(shell, *last_command, (*current_token)->next->token.value);
		*current_token = (*current_token)->next;
	}
	(void)table;
}

void handle_redirect_out_token(t_shell* shell, CommandTable* table, TokenNode** current_token, Command** last_command)
{
	if ((*current_token)->next != NULL && (*current_token)->next->token.type == TOKEN_ARG)
	{
		set_redirect_out(shell, *last_command, (*current_token)->next->token.value, (*current_token)->token.type == TOKEN_REDIR_APPEND);
		*current_token = (*current_token)->next;
	}
	(void)table;
}

void handle_debug_weird_arg(t_shell* shell, CommandTable* table, TokenNode** current_token, Command** last_command)
{
	printToken(*current_token);
	(void)table;
	(void)shell;
	(void)last_command;
}

CommandTable* create_command_table(t_shell *shell, TokenNode* tokens)
{
	CommandTable	*table;
	Command			*current_command;
	Command			*last_command;
	int				pipe_exist;
	TokenNode		*current_token;

	CommandTable* table = initialize_command_table(shell);
	current_command = NULL;
	last_command = NULL;
	pipe_exist = 0;
	current_token = tokens;
	while (current_token != NULL)
	{
		if (tokenHandlers[current_token->token.type])
			tokenHandlers[current_token->token.type](shell, table, &current_token, &last_command);
		if (pipe_exist==1 && current_token->token.type !=  TOKEN_PIPE)
		{
			if (ft_strcmp(current_command->name,"ls") != 0 && ft_strcmp(current_command->name,"echo") != 0)
				if (current_command->fin!=-1)
					current_command->fin=-99;
			pipe_exist =0;
		}
		if (current_token->next && current_token->next->token.type == TOKEN_PIPE)
		{
			if (current_command->fout==0)
				current_command->fout=-99;
			pipe_exist = 1;
		}
		current_token = current_token->next;
	}
	return table;
}
