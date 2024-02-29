/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:22:21 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/23 10:47:32 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_fill_buffer(int fd, ssize_t *bytes_read, char *buffer)
{
	char	*tmp;
	char	*new_buffer;

	tmp = ft_calloc_set_zero(sizeof(char), BUFFER_SIZE + 1);
	if (!tmp)
		return (NULL);
	*bytes_read = read(fd, tmp, BUFFER_SIZE);
	if (*bytes_read == -1 || (*bytes_read == 0 && buffer[0] == '\0'))
	{
		free(buffer);
		free(tmp);
		return (NULL);
	}
	new_buffer = ft_strjoin(buffer, tmp);
	free(buffer);
	free(tmp);
	return (new_buffer);
}

static char	*ft_trim_buffer(char **buffer)
{
	char	*newline;
	char	*result;
	char	*ptr;

	newline = ft_strchr(*buffer, '\n');
	result = (char *)ft_calloc_set_zero(newline - *buffer + 2, sizeof(char));
	if (!result)
		return (NULL);
	ft_strncpy(result, *buffer, newline - *buffer + 1);
	result[newline - *buffer + 1] = '\0';
	ptr = *buffer;
	*buffer = ft_strdup(newline + 1);
	free(ptr);
	ptr = NULL;
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	ssize_t		bytes_read;
	char		*line;

	bytes_read = -1;
	line = NULL;
	if (BUFFER_SIZE < 1 || fd < 0)
		return (NULL);
	if (!buffer)
		buffer = ft_calloc_set_zero(sizeof(char), BUFFER_SIZE +1);
	while (!ft_strchr(buffer, '\n') && bytes_read)
	{
		buffer = (ft_fill_buffer(fd, &bytes_read, buffer));
		if (!buffer)
			break ;
	}
	if (!buffer)
		return (NULL);
	if (ft_strchr(buffer, '\n'))
		return (ft_trim_buffer(&buffer));
	line = ft_strdup(buffer);
	free(buffer);
	buffer = NULL;
	return (line);
}
