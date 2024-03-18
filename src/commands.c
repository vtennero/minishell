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

void link_pipeline_commands(Command* cmd1, Command* cmd2)
{
    cmd1->next = cmd2;
}

// Initialize a new command table
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

// Link two commands in the case of a pipeline
void link_commands(Command* prev_cmd, Command* next_cmd) {
    if (prev_cmd != NULL) {
        prev_cmd->next = next_cmd;
    }
}

void set_redirect_in(t_shell *shell, Command* cmd, char* filename) {
    // if (cmd->redirect_in) free(cmd->redirect_in);
    cmd->redirect_in = shell_strdup(shell, filename);
    int fd = open(filename, O_RDONLY);
    cmd->fin = fd;
}

void set_redirect_out(t_shell *shell, Command* cmd, char* filename, int append) {
    if (!cmd || !filename) return;

    // Free any existing redirection target to avoid memory leaks
    // if (cmd->redirect_out) {
    //     free(cmd->redirect_out);
    //     cmd->redirect_out = NULL;
    // }
    
    // if (cmd->redirect_append) {
    //     free(cmd->redirect_append);
    //     cmd->redirect_append = NULL;
    // }

    // Copy the filename to the appropriate field based on whether it's append or overwrite
    if (append) {
        cmd->redirect_append = shell_strdup(shell, filename);
    } else {

        cmd->redirect_out = shell_strdup(shell, filename);
        int fd = open(filename, O_RDWR | O_CREAT,0666);
        cmd->fout = fd;


    }
}


void free_command_table(CommandTable* table)
{
    if (!table) return;

    Command* cmd = table->head;
    while (cmd)
	{
        Command* next_cmd = cmd->next;

        // Free the command name and arguments
        free(cmd->name);
        for (int i = 0; i < cmd->arg_count; i++) {
            free(cmd->args[i]);
        }
        free(cmd->args);

        // Free redirection targets if they exist
        free(cmd->redirect_in);
        free(cmd->redirect_out);
        free(cmd->redirect_append);

        // Finally, free the command itself
        free(cmd);

        cmd = next_cmd;
    }

    // Free the table structure
    free(table);
}

Command* create_command_entry(t_shell *shell, char* name)
{
    // Use ccalloc to allocate the Command structure, initializing all fields to 0/NULL.
    Command* cmd = (Command*)shell_malloc(shell, sizeof(Command));
    
    if (!cmd) {
        perror("Failed to allocate Command");
        exit(EXIT_FAILURE);
    }

    cmd->name = shell_strdup(shell, name); // shell_strdup allocates and copies the string, so this needs to remain as is.
    cmd->type = CMD_EXTERNAL;

    cmd->fin=0;
    // printf("Initialize cmd %s fin to %d",cmd->name,cmd->fin);

    cmd->fout=0;
    return cmd;
}


void add_argument(t_shell *shell, Command* cmd, char* arg)
{
    // ft_printf("add argument %s\n", arg);
    cmd->args[cmd->arg_count++] = shell_strdup(shell, arg);
    // Assuming cmd->args has been allocated with enough space
    cmd->args[cmd->arg_count] = NULL; // Ensure NULL termination
}


CommandTable    *create_command_table(t_shell *shell, TokenNode* tokens)
{
    CommandTable* table = initialize_command_table(shell);
    Command* current_command = NULL;
    Command* last_command = NULL;

    TokenNode* current_token = tokens;
    int pipe_exist=0;

    // ft_printf("create_command_table current token value |%s| type |%d| \n", current_token->token.value, current_token->token.type);
    while (current_token != NULL)
    {
        // ft_printf("create_command_table while (current_token != NULL) {\n");
        if (current_token->token.type == TOKEN_COMMAND) {
                // ft_printf("create_command_table if (current_token->token.type == TOKEN_COMMAND) { \n");
            // Count arguments for the current command
            int arg_count = 0;
            TokenNode* temp = current_token->next;
            while (temp && temp->token.type == TOKEN_ARG) {
                // ft_printf("create_command_table while (temp && temp->token.type == TOKEN_ARG)\n");
                arg_count++;
                temp = temp->next;
            }

            // Now, create the command entry and allocate args
            current_command = create_command_entry(shell, current_token->token.value);
            // printf("Initialize cmd %s fin to %d",current_command->name,current_command->fin);

            // ft_printf("create_command_table create_command_entry done\n");
            current_command->args = (char**)shell_malloc(shell, (arg_count + 1) * sizeof(char*)); // +1 for NULL terminator

            // Add to command table
            if (table->head == NULL)
                table->head = current_command;
            else if (last_command)
                last_command->next = current_command;
            last_command = current_command;
            table->command_count++;
        } else if(current_token->token.type == TOKEN_ARG)
        // } else if(current_token->token.type == TOKEN_ARG || current_token->token.type == TOKEN_S_Q)
        {
                // ft_printf("create_command_table else if(current_token->token.type == TOKEN_ARG || current_token->token.type == TOKEN_S_Q)\n");
            // ft_printf("create_command_table current_token->token.type == TOKEN_ARG || current_token->token.type == TOKEN_S_Q\n");
            // ft_printf("create_command_table %d\n", current_token->token.value);
            add_argument(shell, current_command, current_token->token.value);
        }
        // ... handle redirections and other token types ...
		else if (current_token->token.type == TOKEN_REDIR_IN)
		{
            if (current_token->next != NULL && current_token->next->token.type == TOKEN_ARG)
			{
                set_redirect_in(shell, current_command, current_token->next->token.value);
                current_token = current_token->next; // Skip the next token since it's part of the redirection
            }
        }
		else if (current_token->token.type == TOKEN_REDIR_OUT || current_token->token.type == TOKEN_REDIR_APPEND)
		{
            if (current_token->next != NULL && current_token->next->token.type == TOKEN_ARG)
			{
                set_redirect_out(shell, current_command, current_token->next->token.value, current_token->token.type == TOKEN_REDIR_APPEND);
                current_token = current_token->next; // Skip the next token since it's part of the redirection
            }
        }
        
        // add if pipe | here @eugene

        if (pipe_exist==1 && current_token->token.type !=  TOKEN_PIPE)

        {
            //   printf("Current cmd is %s and fin is %d",current_command->name,current_command->fin);
            if (ft_strcmp(current_command->name,"ls") != 0 && ft_strcmp(current_command->name,"echo") != 0)
                current_command->fin=-99;
            // printf("Current cmd %s fin to %d",current_command->name,current_command->fin);

            pipe_exist =0;
        }

        if (current_token->next && current_token->next->token.type == TOKEN_PIPE)

        {

            current_command->fout=-99;
                        // printf("Current cmd is %s and fout is %d",current_command->name,current_command->fout);

            pipe_exist =1;
        }

	    // else if (current_token->next->token.type == TOKEN_PIPE)
		// {
        //     if (current_token->next->next != NULL && current_token->next->next->token.type == TOKEN_COMMAND)
		// 	{
        //         // set_redirect_out(shell, current_command, current_token->next->token.value, current_token->token.type == TOKEN_REDIR_APPEND);
        //         current_token = current_token->next; // Skip the next token since it's part of the redirection
        //     }
        // }
        
        current_token = current_token->next;
    }

    return table;
}
