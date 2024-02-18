/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:05:16 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/18 14:33:15 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_command(Command* command)
{
    if (!command) return;


	ft_printf("Executing command: %s arg count %d\n", command->name, command->arg_count);
    // Example stubs for calling built-in commands
    if (strcmp(command->name, "cd") == 0) {
        // Call built-in cd command
        // Assuming the path is the first argument: command->args[1]
        builtin_cd(command->args, command->arg_count);
    } else if (strcmp(command->name, "pwd") == 0) {
        // Call built-in pwd command
        builtin_pwd();
    } else if (strcmp(command->name, "echo") == 0) {
        // Call built-in echo command
		// ft_printf("Executing builtin_echo\n");
		// ft_printf("%s\n", command->args[0]);
        builtin_echo(command->args, command->arg_count);
    } else if (strcmp(command->name, "unset") == 0) {
		// Call built-in unset command
		// Assuming the variable name is the first argument: command->args[1]
		if (command->arg_count > 1) builtin_unset(command->args[1]);
	} else if (strcmp(command->name, "export") == 0) {
		// Call built-in export command
		// Assuming the variable name is the first argument and the value is the second argument
		if (command->arg_count > 1) builtin_export(command->args[1], command->args[2]);
    } else if (strcmp(command->name, "env") == 0) {
		// Call built-in env command
		builtin_env();
    } else if (strcmp(command->name, "exit") == 0) {
        // Call built-in exit command
        builtin_exit();
    } else {
        // For simplicity, we're not implementing external command execution yet.
        printf("Executing external command: %s\n", command->name);
        execute_ext_command(command);
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
