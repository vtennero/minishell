/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:35:26 by cliew             #+#    #+#             */
/*   Updated: 2024/04/10 18:36:49 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_heredocs_in_command_table(t_cmd_table *table)
{
	t_cmd	*cmd;

	cmd = table->head;
	while (cmd)
	{
		if (cmd->heredoc_temp_path)
		{
			cleanup_temp_file(cmd->heredoc_temp_path);
			cmd->heredoc_temp_path = NULL;
		}
		cmd = cmd->next;
	}
}

void	write_heredoc_to_file(int fd, const char *delimiter)
{
	char	*line;

	line = readline("> ");
	while (line != NULL)
	{
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
}

char	*handle_heredoc(const char *delimiter)
{
	char	*temp_path;
	int		fd;

	fd = create_temp_file(&temp_path);
	if (fd == -1)
		return (NULL);
	write_heredoc_to_file(fd, delimiter);
	close(fd);
	return (temp_path);
}

void	prepare_heredocs_in_command_table(t_cmd_table *table)
{
	t_cmd	*cmd;

	cmd = table->head;
	while (cmd)
	{
		if (cmd->heredoc_delimiter)
		{
			cmd->heredoc_temp_path = handle_heredoc(cmd->heredoc_delimiter);
			cmd->fin = -9;
		}
		cmd = cmd->next;
	}
}
