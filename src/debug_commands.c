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

void printTokens(TokenNode* head)
{
    // ft_printf("DEBUG: printTokens\n");
    TokenNode* current = head;
    ft_printf("\n");
    while (current != NULL) {
        ft_printf("Token: %s \t | Type: %d\n", current->token.value, current->token.type);
        current = current->next;
    }
    ft_printf("*********************************\n");
    // ft_printf("DEBUG: printTokens END\n");
}

void print_command(const Command* cmd) {
    if (!cmd) return;

    ft_printf("Command: %s\n", cmd->name);
    ft_printf("Type: %s\n", cmd->type == CMD_BUILTIN ? "Builtin" : "External");

    // Print arguments
    ft_printf("Arguments: ");
    for (int i = 0; i < cmd->arg_count; i++) {
        ft_printf("\"%s\" ", cmd->args[i]);
    }
    ft_printf("\n");

    // Print fin
    // if (cmd->fin)
     {
        ft_printf("Fin is: %d\n", cmd->fin);
    }

    // Print fout
    // if (cmd->fout) 
    {
        ft_printf("Fout is: %d\n", cmd->fout);
    }
    // Print redirections
    if (cmd->redirect_in) {
        ft_printf("Input redirected from: %s\n", cmd->redirect_in);
    }
    if (cmd->redirect_out) {
        ft_printf("Output redirected to: %s\n", cmd->redirect_out);
    }
    if (cmd->redirect_append) {
        ft_printf("Output appended to: %s\n", cmd->redirect_append);
    }

    // If there's a pipe to another command, indicate this
    if (cmd->next) {
        ft_printf("Piped to next command\n");
    }
}

void print_command_table(const CommandTable* table)
{
    if (!table || !table->head) {
        ft_printf("Command table is empty.\n");
        return;
    }
    ft_printf("\nCommand Table (Count: %d):\n", table->command_count);

    Command* current = table->head;
    while (current) {
        print_command(current);
        current = current->next;
        if (current) {
            ft_printf("----------\n"); // Separator between commands in a pipeline
        }
    }
    ft_printf("*********************************\n\n");
    // ft_printf("\n");
}

int is_token_type_present(TokenNode *head, int type)
{
    TokenNode *current = head;
    while (current != NULL) {
        if (current->token.type == type) {
            return 1; // Found the TokenType in the list
        }
        current = current->next;
    }
    return 0; // TokenType not found in the list
}