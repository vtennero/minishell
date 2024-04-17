/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 06:51:32 by cliew             #+#    #+#             */
/*   Updated: 2024/04/16 06:51:32 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_table	*initialize_command_table(t_shell *shell)
{
	t_cmd_table	*table;

	table = (t_cmd_table *)shell_malloc(shell, sizeof(t_cmd_table));
	if (!table)
	{
		ft_putstr_fd("Failed to allocate t_cmd_table", 2);
		exit(EXIT_FAILURE);
	}
	table->head = NULL;
	table->command_count = 0;
	return (table);
}

void	set_rin(t_shell *shell, t_cmd *cmd, char *filename)
{
	int	fd;

	cmd->redirect_in = shell_strdup(shell, filename);
	fd = open(filename, O_RDWR);
	if (cmd->fin != -1)
		cmd->fin = fd;
}

void	set_rout(t_shell *shell, t_cmd *cmd, char *filename, int append)
{
	int	fd;

	if (!cmd || !filename)
		return ;
	fd = 0;
	if (cmd->fout != -1)
	{
		if (cmd->fin == -1)
		{
			return ;
		}
		if (append)
		{
			cmd->redirect_app = shell_strdup(shell, filename);
			fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0666);
		}
		else
		{
			cmd->redirect_out = shell_strdup(shell, filename);
			cmd->redirect_app=NULL;
			fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
		}
		cmd->fout = fd;
	}
}

void	free_command_table(t_cmd_table *table)
{
	t_cmd	*cmd;
	t_cmd	*next_cmd;
	int		i;

	if (!table)
		return ;
	i = 0;
	cmd = table->head;
	while (cmd)
	{
		next_cmd = cmd->next;
		free(cmd->name);
		while (i < cmd->arg_count)
			free(cmd->args[i++]);
		free(cmd->args);
		free(cmd->redirect_in);
		free(cmd->redirect_out);
		free(cmd->redirect_app);
		free(cmd);
		cmd = next_cmd;
	}
	free(table);
}
