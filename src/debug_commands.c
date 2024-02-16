/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:11:36 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/16 13:12:30 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

    ft_printf("Command Table (Count: %d):\n", table->command_count);

    Command* current = table->head;
    while (current) {
        print_command(current);
        current = current->next;
        if (current) {
            ft_printf("----------\n"); // Separator between commands in a pipeline
        }
    }
}
