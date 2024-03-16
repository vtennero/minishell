/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:05:16 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/23 14:25:59 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_command(t_shell *shell, Command* command)
{
    if (!command) return;

    (void)shell;
	// ft_printf("Executing command: %s arg count %d\n", command->name, command->arg_count);
    // Example stubs for calling built-in commands
    if (ft_strcmp(command->name, "cd") == 0) {
        // Call built-in cd command
        // Assuming the path is the first argument: command->args[1]
        builtin_cd(shell, command->args, command->arg_count);
    } else if (ft_strcmp(command->name, "pwd") == 0) {
        // Call built-in pwd command
        builtin_pwd();
    } else if (ft_strcmp(command->name, "echo") == 0) {
        // Call built-in echo command
		// ft_printf("Executing builtin_echo\n");
		// ft_printf("%s\n", command->args[0]);
        builtin_echo(shell, command->args, command->arg_count);
    } else if (ft_strcmp(command->name, "unset") == 0) {
		// Call built-in unset command
		// Assuming the variable name is the first argument: command->args[1]
		builtin_unset(shell, command->args, command->arg_count);
	} else if (ft_strcmp(command->name, "export") == 0) {
		// Call built-in export command
		builtin_export(shell, command->args, command->arg_count);
    } else if (ft_strcmp(command->name, "env") == 0) {
		// Call built-in env command
		builtin_env();
    } else if (ft_strcmp(command->name, "exit") == 0) {
        // Call built-in exit command

        builtin_exit(shell, command->args, command->arg_count);
    } else {
        // printf("Executing external command: %s\n", command->name);
        execute_ext_command(shell, command);
        // Here you would eventually include the logic to fork a process and use execvp or similar to run the command
    }
}

void execute_command_table(t_shell *shell, CommandTable* table)
{
    if (!table) return;

    Command* current = table->head;
    (void)shell;
    while (current != NULL)
    {
        execute_command(shell, current); // Execute the current command
        current = current->next;  // Move to the next command in the list
    }
}
