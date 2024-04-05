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

// void	execute_command(t_shell *shell, Command *command)
// {
// 	if (!command)
// 		return ;
// 	if (ft_strcmp(command->name, "cd") == 0)
// 		builtin_cd(shell, command->args, command->arg_count);
// 	else if (ft_strcmp(command->name, "pwd") == 0)
// 		builtin_pwd();
// 	else if (ft_strcmp(command->name, "echo") == 0)
// 		builtin_echo(shell, command->args, command->arg_count);
// 	else if (ft_strcmp(command->name, "unset") == 0)
// 		builtin_unset(shell, command->args, command->arg_count);
// 	else if (ft_strcmp(command->name, "export") == 0)
// 		builtin_export(shell, command->args, command->arg_count);
// 	else if (ft_strcmp(command->name, "env") == 0)
// 		builtin_env(shell);
// 	else if (ft_strcmp(command->name, "exit") == 0)
// 		builtin_exit(shell, command->args, command->arg_count);
// 	else
// 		execute_ext_command(shell, command);
// }

void	execute_command_table(t_shell *shell, CommandTable *table)
{
	Command	*current;

	current = table->head;
	(void)shell;
	// if (is_token_type_present(shell->token_head, TOKEN_REDIR_HEREDOC))
	// {
	//     ft_printf("heredoc detected\n");
	// }
	// else
	// prepare_heredocs_in_command_table(table);
	// print_command_table(table);
	pipex(current, shell);
	cleanup_heredocs_in_command_table(table);
	// while (current != NULL)
	// {
	//     execute_command(shell, current); // Execute the current command
	//     current = current->next;  // Move to the next command in the list
	// }
}
