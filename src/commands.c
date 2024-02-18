/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:01:04 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/18 15:15:46 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void link_pipeline_commands(Command* cmd1, Command* cmd2) {
    cmd1->next = cmd2;
}

// Initialize a new command table
CommandTable* initialize_command_table()
{
    CommandTable* table = (CommandTable*)malloc(sizeof(CommandTable));
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

void set_redirect_in(Command* cmd, char* filename) {
    if (cmd->redirect_in) free(cmd->redirect_in);
    cmd->redirect_in = strdup(filename);
}

void set_redirect_out(Command* cmd, char* filename, int append) {
    if (!cmd || !filename) return;

    // Free any existing redirection target to avoid memory leaks
    if (cmd->redirect_out) {
        free(cmd->redirect_out);
        cmd->redirect_out = NULL;
    }
    
    if (cmd->redirect_append) {
        free(cmd->redirect_append);
        cmd->redirect_append = NULL;
    }

    // Copy the filename to the appropriate field based on whether it's append or overwrite
    if (append) {
        cmd->redirect_append = strdup(filename);
    } else {
        cmd->redirect_out = strdup(filename);
    }
}



// void add_argument1(Command* cmd, char* arg) {
//     cmd->args = realloc(cmd->args, (cmd->arg_count + 2) * sizeof(char*));
//     cmd->args[cmd->arg_count++] = strdup(arg);
//     cmd->args[cmd->arg_count] = NULL; // Ensure NULL termination
// }


// Command* create_command_entry1(char* name) {
//     Command* cmd = (Command*)malloc(sizeof(Command));
//     cmd->name = strdup(name);
//     cmd->args = NULL;
//     cmd->arg_count = 0;
//     cmd->redirect_in = NULL;
//     cmd->redirect_out = NULL;
//     cmd->redirect_append = NULL;
//     cmd->next = NULL;
//     cmd->type = CMD_EXTERNAL; // Default to external, could be updated based on the command name
//     return cmd;
// }

// // New function to create the command table from a list of tokens
// CommandTable* create_command_table1(TokenNode* tokens)
// {
//     CommandTable* table = initialize_command_table();
//     Command* current_command = NULL;
//     Command* last_command = NULL;

//     TokenNode* current_token = tokens;
//     while (current_token != NULL)
// 	{
//         if (current_token->token.type == TOKEN_COMMAND)
// 		{
//             if (current_command != NULL)
// 				// Link the previous command to the new one if it's not the first command
//                 link_commands(last_command, current_command);
//             current_command = create_command_entry(current_token->token.value);
//             if (table->head == NULL)
//                 table->head = current_command;
//             last_command = current_command;
//             table->command_count++;
//         }
// 		else if (current_token->token.type == TOKEN_ARG)
// 		{
//             add_argument(current_command, current_token->token.value);
//         }
// 		else if (current_token->token.type == TOKEN_REDIR_IN)
// 		{
//             if (current_token->next != NULL && current_token->next->token.type == TOKEN_ARG)
// 			{
//                 set_redirect_in(current_command, current_token->next->token.value);
//                 current_token = current_token->next; // Skip the next token since it's part of the redirection
//             }
//         }
// 		else if (current_token->token.type == TOKEN_REDIR_OUT || current_token->token.type == TOKEN_REDIR_APPEND)
// 		{
//             if (current_token->next != NULL && current_token->next->token.type == TOKEN_ARG)
// 			{
//                 set_redirect_out(current_command, current_token->next->token.value, current_token->token.type == TOKEN_REDIR_APPEND);
//                 current_token = current_token->next; // Skip the next token since it's part of the redirection
//             }
//         }
// 		else
// 			printf("Error: Unknown token type\n");
//         // Handling pipes and linking commands will be similar to handling redirections but may involve creating a new command structure and adjusting file descriptors accordingly
//         current_token = current_token->next;
//     }

//     return table;
// }

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

Command* create_command_entry(char* name) {
    // Use calloc to allocate the Command structure, initializing all fields to 0/NULL.
    Command* cmd = (Command*)calloc(1, sizeof(Command));
    if (!cmd) {
        perror("Failed to allocate Command");
        exit(EXIT_FAILURE);
    }

    cmd->name = strdup(name); // strdup allocates and copies the string, so this needs to remain as is.
    // No need to explicitly set the other fields to NULL or 0, calloc has already done this.
    cmd->type = CMD_EXTERNAL; // Explicitly setting type, as calloc initializes it to 0, which may not correspond to CMD_EXTERNAL depending on the enum values.

    return cmd;
}


void add_argument(Command* cmd, char* arg)
{
    cmd->args[cmd->arg_count++] = strdup(arg);
    // Assuming cmd->args has been allocated with enough space
    cmd->args[cmd->arg_count] = NULL; // Ensure NULL termination
}


CommandTable* create_command_table(TokenNode* tokens) {
    CommandTable* table = initialize_command_table();
    Command* current_command = NULL;
    Command* last_command = NULL;

    TokenNode* current_token = tokens;
    while (current_token != NULL) {
        if (current_token->token.type == TOKEN_COMMAND) {
            // Count arguments for the current command
            int arg_count = 0;
            TokenNode* temp = current_token->next;
            while (temp && temp->token.type == TOKEN_ARG) {
                arg_count++;
                temp = temp->next;
            }

            // Now, create the command entry and allocate args
            current_command = create_command_entry(current_token->token.value);
            current_command->args = (char**)calloc(arg_count + 1, sizeof(char*)); // +1 for NULL terminator

            // Add to command table
            if (table->head == NULL) table->head = current_command;
            else if (last_command) last_command->next = current_command;
            last_command = current_command;
            table->command_count++;
        } else if (current_token->token.type == TOKEN_ARG) {
            add_argument(current_command, current_token->token.value);
        }
        // ... handle redirections and other token types ...
		else if (current_token->token.type == TOKEN_REDIR_IN)
		{
            if (current_token->next != NULL && current_token->next->token.type == TOKEN_ARG)
			{
                set_redirect_in(current_command, current_token->next->token.value);
                current_token = current_token->next; // Skip the next token since it's part of the redirection
            }
        }
		else if (current_token->token.type == TOKEN_REDIR_OUT || current_token->token.type == TOKEN_REDIR_APPEND)
		{
            if (current_token->next != NULL && current_token->next->token.type == TOKEN_ARG)
			{
                set_redirect_out(current_command, current_token->next->token.value, current_token->token.type == TOKEN_REDIR_APPEND);
                current_token = current_token->next; // Skip the next token since it's part of the redirection
            }
        }
        current_token = current_token->next;
    }

    return table;
}
