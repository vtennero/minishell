/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:57:09 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 10:57:09 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_temp_file_path(const char *basePath, const char *timeStr,
		const char *counterStr)
{
	int		buffer_size;
	char	*temp_file_path;
	char	*ptr;

	buffer_size = strlen(basePath) + strlen(timeStr) + strlen(counterStr) + 2;
	temp_file_path = malloc(buffer_size);
	if (temp_file_path == NULL)
	{
		perror("malloc failed");
		return (NULL);
	}
	ptr = temp_file_path;
	memcpy(ptr, basePath, strlen(basePath));
	ptr += strlen(basePath);
	memcpy(ptr, timeStr, strlen(timeStr));
	ptr += strlen(timeStr);
	*ptr = '_';
	ptr++;
	memcpy(ptr, counterStr, strlen(counterStr) + 1);
	return (temp_file_path);
}

int	create_and_unlink_temp_file(const char *tempFilePath)
{
	int	fd;

	fd = open(tempFilePath, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (fd == -1)
	{
		perror("Cannot create temporary file for heredoc");
		return (-1);
	}
	return (fd);
}

void	free_two(void *ptr1, void *ptr2)
{
	if (ptr1 != NULL)
		free(ptr1);
	if (ptr2 != NULL)
		free(ptr2);
}

int	create_temp_file(char **tempFilePath)
{
	static int	counter;
	char		*time_str;
	char		*counter_str;
	int			fd;

	counter = 0;
	time_str = ft_itoa((int)time(NULL));
	counter_str = ft_itoa(counter++);
	if (time_str == NULL || counter_str == NULL)
	{
		free_two(time_str, counter_str);
		return (-1);
	}
	*tempFilePath = create_temp_file_path("/tmp/", time_str, counter_str);
	free_two(time_str, counter_str);
	if (*tempFilePath == NULL)
		return (-1);
	fd = create_and_unlink_temp_file(*tempFilePath);
	if (fd == -1)
	{
		free(*tempFilePath);
		*tempFilePath = NULL;
	}
	return (fd);
}

void	cleanup_temp_file(char *tempFilePath)
{
	if (tempFilePath)
	{
		unlink(tempFilePath);
		free(tempFilePath);
	}
}

void	cleanup_heredocs_in_command_table(CommandTable *table)
{
	Command	*cmd;

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

void	prepare_heredocs_in_command_table(CommandTable *table)
{
	Command	*cmd;

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
