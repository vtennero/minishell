/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:27:16 by cliew             #+#    #+#             */
/*   Updated: 2024/04/14 08:48:28 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	assign_cmd_args(t_shell *shell, t_cmd *command, char **envp)
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

int	check_error(t_cmd *cmd, t_shell *shell, int parent)
{
	if (cmd->name==NULL)
	{
		if (cmd->fin == -1 || cmd->fout == -1)
			ft_putstr_fd(" File not exists/permission error\n", 2);
		return(1);
	}
	if (is_directory(cmd->name) == 1)
	{
		ft_putstr_fd(" Is a directory\n", 2);
		shell->last_exit_status = 126;
		return (1);
	}
	if (ft_strcmp(cmd->name, "") == 0)
	{
		shell->last_exit_status = 0;
		return (1);
	}
	if (parent && builtin_cmd(cmd, shell))

			return (1);
	return (0);
}

char	*check_path(char **envp)

{
	while (*envp++ != NULL)
	{
		if (ft_strnstr(*envp, "PATH=", 5) != NULL)
		{
			return (*(envp));
		}
	}
	return (NULL);
}

char	*locate_cmd(char **paths, char *cmd)
{
	char	*_cmd;
	char	*cmd_path;

	if (access(cmd, X_OK) >= 0)
		return (ft_strdup(cmd));
	_cmd = ft_strjoin("/", cmd);
	cmd_path = NULL;
	while (paths && *paths)
	{
		cmd_path = ft_strjoin(*paths, _cmd);
		if (access(cmd_path, X_OK) >= 0)
		{
			free(_cmd);
			return (cmd_path);
		}
		free(cmd_path);
		cmd_path = NULL;
		paths++;
	}
	free(_cmd);
	return (cmd_path);
}
