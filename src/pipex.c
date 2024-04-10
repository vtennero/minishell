/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:17:01 by cliew             #+#    #+#             */
/*   Updated: 2024/04/10 15:08:58 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(Command *cmd)
{
	free(cmd->cmd_path);
}

int	builtin_cmd(Command *command, t_shell *shell)
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

int	custom_cmd(char **cmd_args, char *cmd_path, Command *cmd, t_shell *shell)
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

int	run_cmd(Command *command, t_shell *shell)
{
	int	status;

	status = 0;
	if (!custom_cmd(command->execv_args, command->cmd_path, command, shell))
	{
		shell->last_exit_status = status;
		return (ft_puterr(ft_strjoin_nconst(command->name, ERR_INVALID_CMD),
				127));
	}
	return (0);
}

void	pipe_heredoc(Command *cmd, t_shell *shell)
{
	int	fd;

	cmd->redirect_in = shell_strdup(shell, cmd->heredoc_temp_path);
	fd = open(cmd->heredoc_temp_path, O_RDWR);
	if (fd != (-1))
		cmd->fin = fd;
	dup2(cmd->fin, STDIN_FILENO);
	close(cmd->fin);
}

void	check_finfout(int prev_pipe, Command *cmd, t_shell *shell)
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

void	clean_fd(t_shell *shell, int std_in, int std_out, Command *cmd)
{
	close(shell->pipefd[0]);
	close(shell->pipefd[1]);
	if (dup2(std_in, STDIN_FILENO) == -1)
		ft_putstr_fd("dup2", 2);
	if (dup2(std_out, STDOUT_FILENO) == -1)
		ft_putstr_fd("dup2", 2);
	free_cmd(cmd);
}

void	handle_status_error(int status, Command *cmd, t_shell *shell)
{
	int	status2;

	if (WIFEXITED(status) && ft_strcmp(cmd->name, "exit") != 0)
	{
		status2 = WEXITSTATUS(status);
		shell->last_exit_status = status2;
	}
	else
		shell->last_exit_status = status;
}

int	assign_cmd_args(t_shell *shell, Command *command, char **envp)
{
	int		i;
	char	**paths;

	i = 1;
	paths = find_cmd_paths(envp);
	command->cmd_path = locate_cmd(paths, command->name);
	free_array(paths);
	command->execv_args = shell_malloc(shell, (command->arg_count + 2)
			* sizeof(char *));
	command->execv_args[0] = command->name;
	while (i <= command->arg_count)
	{
		command->execv_args[i] = command->args[i - 1];
		i++;
	}
	command->execv_args[i] = 0;
	return (0);
}

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (-1);
	if (!S_ISDIR(path_stat.st_mode))
		return (0);
	return (1);
}

int	check_error(Command *cmd, t_shell *shell, int parent)
{
	if (is_directory(cmd->name) == 1)
	{
		ft_putstr_fd("is a directory\n", 2);
		shell->last_exit_status = 126;
		return (1);
	}
	if (parent)
	{
		if (builtin_cmd(cmd, shell))
			return (1);
	}
	return (0);
}

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

void	check_child_error(t_shell *shell, Command *cmd, char *error)
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
		error = ft_strjoin_nconst(cmd->name, " : Command not found");
		shell->last_exit_status = 127;
	}
	if (error != NULL)
	{
		ft_putstr_fd(error, 2);
		free(error);
		exit(1);
	}
}

int	execute_command_pipex(int prev_pipe, Command *cmd, t_shell *shell,
		int parent)
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

void	last_pipe(t_shell *shell, Command *cmd, int prev_pipe, int *status)
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

int	pipex(Command *cmd, t_shell *shell)
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
