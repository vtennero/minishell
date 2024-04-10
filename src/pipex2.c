/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:34:20 by cliew             #+#    #+#             */
/*   Updated: 2024/04/10 18:34:41 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_cmd *cmd)
{
	free(cmd->cmd_path);
}

int	builtin_cmd(t_cmd *command, t_shell *shell)
{
	int	exit_code;

	exit_code = -99999;
	if (ft_strcmp(command->name, "cd") == 0)
		exit_code = builtin_cd(shell, command->args, command->arg_count);
	else if (ft_strcmp(command->name, "unset") == 0)
		exit_code = builtin_unset(shell, command->args, command->arg_count);
	else if (ft_strcmp(command->name, "export") == 0 && command->fout != -99)
		exit_code = builtin_export(shell, command->args, command->arg_count);
	else if (ft_strcmp(command->name, "exit") == 0)
		exit_code = builtin_exit(shell, command->args, command->arg_count);
	if (exit_code == -99999)
		return (0);
	shell->last_exit_status = exit_code;
	return (1);
}

void	run_execv(char **cmd_args, char *cmd_path, t_shell *shell)
{
	execve(cmd_path, cmd_args, shell->envp);
	free(cmd_path);
	free_array(cmd_args);
	exit(EXIT_FAILURE);
}

int	custom_cmd(char **cmd_args, char *cmd_path, t_cmd *cmd, t_shell *shell)
{
	int	exit_code;

	exit_code = -999;
	if (ft_strcmp(cmd->name, "pwd") == 0)
		exit_code = builtin_pwd();
	else if (ft_strcmp(cmd->name, "echo") == 0)
		exit_code = builtin_echo(shell, cmd->args, cmd->arg_count, cmd);
	else if (ft_strcmp(cmd->name, "env") == 0)
		exit_code = builtin_env(shell);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		exit_code = builtin_cd(shell, cmd->args, cmd->arg_count);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		exit_code = builtin_unset(shell, cmd->args, cmd->arg_count);
	else if (ft_strcmp(cmd->name, "export") == 0)
		exit_code = builtin_export(shell, cmd->args, cmd->arg_count);
	else if (ft_strcmp(cmd->name, "exit") == 0)
		exit_code = builtin_exit(shell, cmd->args, cmd->arg_count);
	if (exit_code != -999)
		exit(exit_code);
	else if (cmd_path)
		run_execv(cmd_args, cmd_path, shell);
	else
		return (0);
	return (0);
}

void	prepend_linecount(t_shell *shell)
{
	char	*linecount;

	if (shell->is_interactive == 0)
	{
		linecount = ft_itoa(shell->line_count);
		ft_putstr_fd(shell->readfile, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("line ", 2);
		ft_putstr_fd(linecount, 2);
		ft_putstr_fd(": ", 2);
		free(linecount);
	}
}
