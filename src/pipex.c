/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:17:01 by cliew             #+#    #+#             */
/*   Updated: 2024/04/10 18:13:51 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_custom_cmd(char *name)
{
	if ((ft_strcmp(name, "cd") == 0) || (ft_strcmp(name, "env") == 0)
		|| (ft_strcmp(name, "exit") == 0) || (ft_strcmp(name, "unset") == 0)
		|| (ft_strcmp(name, "export") == 0) || (ft_strcmp(name, "echo") == 0)
		|| (ft_strcmp(name, "pwd") == 0))
		return (1);
	else
		return (0);
}

void	check_child_error(t_shell *shell, t_cmd *cmd, char *error)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->fin == -1)
		error = ft_strjoin_nconst(cmd->redirect_in,
				" : File not exists/permission error");
	else if (cmd->fout == -1)
		ft_puterr(ft_strjoin_nconst(cmd->redirect_out,
				" : File not exists/permission error"), 1);
	else if (!find_env_var(shell->env_head, "PATH")
		&& !is_custom_cmd(cmd->name))
	{
		error = ft_strjoin_nconst(cmd->name, " : t_cmd not found");
		shell->last_exit_status = 127;
	}
	if (error != NULL)
	{
		ft_putstr_fd(error, 2);
		free(error);
		exit(1);
	}
}

int	execute_command_pipex(int prev_pipe, t_cmd *cmd, t_shell *shell, int parent)
{
	pid_t	pid;
	char	*error;

	error = NULL;
	if (check_error(cmd, shell, parent))
		return (1);
	assign_cmd_args(shell, cmd, shell->envp);
	pid = fork();
	if (pid < 0)
		return (write(STDOUT_FILENO, "Error forking\n", 15));
	if (pid == 0)
	{
		check_child_error(shell, cmd, error);
		check_finfout(prev_pipe, cmd, shell);
		run_cmd(cmd, shell);
		exit(1);
	}
	else
	{
		shell->pid = pid;
		return (0);
	}
}

void	last_pipe(t_shell *shell, t_cmd *cmd, int prev_pipe, int *status)
{
	if (shell->table->command_count != 1)
	{
		if (!execute_command_pipex(prev_pipe, cmd, shell, 0))
		{
			waitpid(shell->pid, status, WUNTRACED);
			handle_status_error(*status, cmd, shell);
		}
	}
	else
	{
		if (!execute_command_pipex(prev_pipe, cmd, shell, 1))
		{
			waitpid(shell->pid, status, WUNTRACED);
			handle_status_error(*status, cmd, shell);
		}
	}
}

int	pipex(t_cmd *cmd, t_shell *shell)
{
	int	status;
	int	prev_pipe;

	prev_pipe = cmd->fin;
	while (cmd->next)
	{
		if (pipe(shell->pipefd) < 0)
			return (write(STDOUT_FILENO, "Error creating pipe\n", 20));
		if (cmd->fout == 0)
		{
			if (!execute_command_pipex(prev_pipe, cmd, shell, 0))
				waitpid(shell->pid, &status, WUNTRACED);
		}
		else
			execute_command_pipex(prev_pipe, cmd, shell, 0);
		prev_pipe = shell->pipefd[0];
		close(shell->pipefd[1]);
		free_cmd(cmd);
		*cmd = *(cmd->next);
	}
	last_pipe(shell, cmd, prev_pipe, &status);
	clean_fd(shell, shell->std_in, shell->std_out, cmd);
	return (0);
}
