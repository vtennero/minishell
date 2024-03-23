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
	NULL, // Assuming 0 is an invalid/unused token type
	handle_command_token, // Assuming 1 corresponds to COMMAND
	handle_arg_token,     // Assuming 2 corresponds to ARG
	handle_redirect_in_token,
	handle_redirect_out_token,
	// Populate other handlers according to the token type values...
};

CommandTable* initialize_command_table(t_shell *shell)
{
	CommandTable* table = (CommandTable*)shell_malloc(shell, sizeof(CommandTable));
	if (!table) {
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

Command* create_command_entry(t_shell *shell, char* name)
{
	// Use ccalloc to allocate the Command structure, initializing all fields to 0/NULL.
	Command* cmd = (Command*)shell_malloc(shell, sizeof(Command));
	
	if (!cmd) {
		perror("Failed to allocate Command");
		exit(EXIT_FAILURE);
	}
	cmd->name = shell_strdup(shell, name);
	cmd->fin=0;
	cmd->fout=0;
	return (cmd);
}

void add_argument(t_shell *shell, Command* cmd, char* arg)
{
	cmd->args[cmd->arg_count++] = shell_strdup(shell, arg);
	cmd->args[cmd->arg_count] = NULL; // Ensure NULL termination
}

void handle_command_token(t_shell* shell, CommandTable* table, TokenNode** current_token, Command** last_command)
{
	Command* current_command = create_command_entry(shell, (*current_token)->token.value);
	int arg_count = 0;

	TokenNode* temp = (*current_token)->next;
	while (temp && temp->token.type == TOKEN_ARG) {
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
}

void handle_arg_token(t_shell* shell, CommandTable* table, TokenNode** current_token, Command** last_command)
{
	if (*last_command != NULL)
		add_argument(shell, *last_command, (*current_token)->token.value);
	(void)table;
}

void handle_redirect_in_token(t_shell* shell, CommandTable* table, TokenNode** current_token, Command** last_command) {
	if ((*current_token)->next != NULL && (*current_token)->next->token.type == TOKEN_ARG) {
		set_redirect_in(shell, *last_command, (*current_token)->next->token.value);
		*current_token = (*current_token)->next; // Skip the next token
	}
	(void)table;
}

void handle_redirect_out_token(t_shell* shell, CommandTable* table, TokenNode** current_token, Command** last_command) {
	if ((*current_token)->next != NULL && (*current_token)->next->token.type == TOKEN_ARG) {
		set_redirect_out(shell, *last_command, (*current_token)->next->token.value, (*current_token)->token.type == TOKEN_REDIR_APPEND);
		*current_token = (*current_token)->next; // Skip the next token
	}
	(void)table;
}

CommandTable* create_command_table(t_shell *shell, TokenNode* tokens)
{
	CommandTable* table = initialize_command_table(shell);
	Command* current_command = NULL;
	Command* last_command = NULL;
	int pipe_exist = 0;
	for (TokenNode* current_token = tokens; current_token != NULL; current_token = current_token->next)
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
			pipe_exist =1;
		}
	}
	return table;
}

// CommandTable    *create_command_table(t_shell *shell, TokenNode* tokens)
// {
//     CommandTable* table = initialize_command_table(shell);
//     Command* current_command = NULL;
//     Command* last_command = NULL;

//     TokenNode* current_token = tokens;
//     int pipe_exist=0;

//     // ft_printf("create_command_table current token value |%s| type |%d| \n", current_token->token.value, current_token->token.type);
//     while (current_token != NULL)
//     {
//         // ft_printf("create_command_table while (current_token != NULL) {\n");
//         if (current_token->token.type == TOKEN_COMMAND) {
//                 // ft_printf("create_command_table if (current_token->token.type == TOKEN_COMMAND) { \n");
//             // Count arguments for the current command
//             int arg_count = 0;
//             TokenNode* temp = current_token->next;
//             while (temp && temp->token.type == TOKEN_ARG) {
//                 // ft_printf("create_command_table while (temp && temp->token.type == TOKEN_ARG)\n");
//                 arg_count++;
//                 temp = temp->next;
//             }

//             // Now, create the command entry and allocate args
//             current_command = create_command_entry(shell, current_token->token.value);
//             // printf("Initialize cmd %s fin to %d",current_command->name,current_command->fin);

//             // ft_printf("create_command_table create_command_entry done\n");
//             current_command->args = (char**)shell_malloc(shell, (arg_count + 1) * sizeof(char*)); // +1 for NULL terminator

//             // Add to command table
//             if (table->head == NULL)
//                 table->head = current_command;
//             else if (last_command)
//                 last_command->next = current_command;
//             last_command = current_command;
//             table->command_count++;
//         } else if(current_token->token.type == TOKEN_ARG)
//         // } else if(current_token->token.type == TOKEN_ARG || current_token->token.type == TOKEN_S_Q)
//         {
//                 // ft_printf("create_command_table else if(current_token->token.type == TOKEN_ARG || current_token->token.type == TOKEN_S_Q)\n");
//             // ft_printf("create_command_table current_token->token.type == TOKEN_ARG || current_token->token.type == TOKEN_S_Q\n");
//             // ft_printf("create_command_table %d\n", current_token->token.value);
//             add_argument(shell, current_command, current_token->token.value);
//         }
//         // ... handle redirections and other token types ...
// 		else if (current_token->token.type == TOKEN_REDIR_IN)
// 		{
//             if (current_token->next != NULL && current_token->next->token.type == TOKEN_ARG)
// 			{
//                 set_redirect_in(shell, current_command, current_token->next->token.value);
//                 current_token = current_token->next; // Skip the next token since it's part of the redirection
//             }
//         }
// 		else if (current_token->token.type == TOKEN_REDIR_OUT || current_token->token.type == TOKEN_REDIR_APPEND)
// 		{
//             if (current_token->next != NULL && current_token->next->token.type == TOKEN_ARG)
// 			{
//                 set_redirect_out(shell, current_command, current_token->next->token.value, current_token->token.type == TOKEN_REDIR_APPEND);
//                 current_token = current_token->next; // Skip the next token since it's part of the redirection
//             }
//         }
		
//         // add if pipe | here @eugene



// 	    // else if (current_token->next->token.type == TOKEN_PIPE)
// 		// {
//         //     if (current_token->next->next != NULL && current_token->next->next->token.type == TOKEN_COMMAND)
// 		// 	{
//         //         // set_redirect_out(shell, current_command, current_token->next->token.value, current_token->token.type == TOKEN_REDIR_APPEND);
//         //         current_token = current_token->next; // Skip the next token since it's part of the redirection
//         //     }
//         // }
		
//         current_token = current_token->next;
//     }

//     return table;
// }