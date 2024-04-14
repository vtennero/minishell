/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:27:00 by cliew             #+#    #+#             */
/*   Updated: 2024/04/14 10:53:35 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_cmd(t_cmd *command, t_shell *shell)
{
	int	status;

	status = 0;
	if (!custom_cmd(command->execv_args, command->cmd_path, command, shell))
	{
		shell->last_exit_status = status;
		prepend_linecount(shell);
		return (ft_puterr(ft_strjoin_nconst(command->name, ERR_INVALID_CMD),
				127));
	}
	return (0);
}

void	pipe_heredoc(t_cmd *cmd, t_shell *shell)
{
	int	fd;

	cmd->redirect_in = shell_strdup(shell, cmd->heredoc_temp_path);
	fd = open(cmd->heredoc_temp_path, O_RDWR);
	if (fd != (-1))
		cmd->fin = fd;
	dup2(cmd->fin, STDIN_FILENO);
	close(cmd->fin);
}

void	check_finfout(int prev_pipe, t_cmd *cmd, t_shell *shell)
{
	if (cmd->fin == -99 && prev_pipe != STDIN_FILENO)
		dup2(prev_pipe, STDIN_FILENO);
	else if (cmd->fin == -9)
		pipe_heredoc(cmd, shell);
	else if (cmd->fin != 0)
	{
		dup2(cmd->fin, STDIN_FILENO);
		close(cmd->fin);
	}
	else if (cmd->fin == 0)
		dup2(shell->std_in, STDIN_FILENO);
	if (cmd->fout == -99 && shell->pipefd[1] != STDOUT_FILENO)
	{
		dup2(shell->pipefd[1], STDOUT_FILENO);
		close(shell->pipefd[1]);
	}
	else if (cmd->fout != 1 && cmd->fout != 0 && cmd->fout != -99)
		dup2(cmd->fout, STDOUT_FILENO);
	else if (cmd->fout == 0)
		dup2(shell->std_out, STDOUT_FILENO);
	if (shell->pipefd[0] != STDOUT_FILENO && shell->pipefd[0] != STDIN_FILENO)
		close(shell->pipefd[0]);
}

void	clean_fd(t_shell *shell, int std_in, int std_out, t_cmd *cmd)
{
	close(shell->pipefd[0]);
	close(shell->pipefd[1]);
	if (dup2(std_in, STDIN_FILENO) == -1)
		ft_putstr_fd("dup2", 2);
	if (dup2(std_out, STDOUT_FILENO) == -1)
		ft_putstr_fd("dup2", 2);
	free_cmd(cmd);
}

void	handle_status_error(int status, t_cmd *cmd, t_shell *shell)
{
	int	status2;

	if (WIFEXITED(status) && \
		(cmd->name == NULL || ft_strcmp(cmd->name, "exit") != 0))
	{
		status2 = WEXITSTATUS(status);
		shell->last_exit_status = status2;
	}
	else
		shell->last_exit_status = status;
}
