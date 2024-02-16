/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:05:16 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/16 14:34:18 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_command(Command* command)
{
    if (!command) return;


	// ft_printf("Executing command: %s\n", command->name);
    // Example stubs for calling built-in commands
    if (strcmp(command->name, "cd") == 0) {
        // Call built-in cd command
        // Assuming the path is the first argument: command->args[1]
        if (command->arg_count > 1) builtin_cd(command->args[1]);
    } else if (strcmp(command->name, "pwd") == 0) {
        // Call built-in pwd command
        builtin_pwd();
    } else if (strcmp(command->name, "echo") == 0) {
        // Call built-in echo command
		// ft_printf("Executing builtin_echo\n");
		// ft_printf("%s\n", command->args[0]);
        builtin_echo(command->args, command->arg_count);
    } else if (strcmp(command->name, "exit") == 0) {
        // Call built-in exit command
        builtin_exit();
    } else {
        // For simplicity, we're not implementing external command execution yet.
        printf("Executing external command: %s\n", command->name);
        // Here you would eventually include the logic to fork a process and use execvp or similar to run the command
    }
}

void execute_command_table(CommandTable* table)
{
    if (!table) return;

    Command* current = table->head;
    while (current != NULL) {
        execute_command(current); // Execute the current command
        current = current->next;  // Move to the next command in the list
    }
}
