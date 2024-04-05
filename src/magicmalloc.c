/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magicmalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:42:10 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/23 14:26:28 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*shell_strjoin(t_shell *shell, char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	str = NULL;
	if (s1 && s2)
	{
		str = shell_malloc(shell, (ft_strlen(s1) + ft_strlen(s2) + 1));
		if (!str)
			return (NULL);
		while (s1[i])
		{
			str[i] = s1[i];
			i++;
		}
		while (s2[j])
		{
			str[i++] = s2[j++];
		}
		str[i] = '\0';
	}
	return (str);
}

char	*shell_strdup(t_shell *shell, const char *s)
{
	char	*newStr;

	size_t len = ft_strlen(s) + 1;
	
	newStr = (char *)shell_malloc(shell, len);
	if (newStr)
	{
		ft_strncpy(newStr, (char *)s, len);
		newStr[len - 1] = '\0'; // Ensure null termination
	}
	return (newStr);
}

char	*shell_strndup(t_shell *shell, const char *s, size_t n)
{
	size_t	len;
	char	*newStr;

	len = strlen(s);
	if (len > n)
	{
		len = n;
	}
	newStr = (char *)shell_malloc(shell, len + 1);
	if (newStr)
	{
		ft_strncpy(newStr, s, len);
		newStr[len] = '\0';
	}
	return (newStr);
}

void	*shell_malloc(t_shell *shell, size_t size)
{
	void	*ptr;
	MemNode	*node;

	ptr = ft_calloc(1, size);
	if (ptr)
	{
		node = (MemNode *)malloc(sizeof(MemNode));
		if (node)
		{
			node->ptr = ptr;
			node->next = shell->mem_tracker.head;
			shell->mem_tracker.head = node;
		}
		else
		{
			free(ptr);
			return (NULL);
		}
	}
	return (ptr);
}

void	shell_free(t_shell *shell, void *ptr)
{
	MemNode	**current;
	MemNode	*entry;

	current = &shell->mem_tracker.head;
	while (*current)
	{
		entry = *current;
		if (entry->ptr == ptr)
		{
			*current = entry->next;
			free(entry->ptr);
			free(entry);
			return ;
		}
		current = &entry->next;
	}
}

void	shell_cleanup(t_shell *shell)
{
	MemNode	*current;
	MemNode	*next;

	current = shell->mem_tracker.head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	shell->mem_tracker.head = NULL;
}

void	shexit(t_shell *shell, int exit_code)
{
	int	exit_s;

	exit_s = shell->last_exit_status;
	shell_cleanup(shell);
	free(shell);
	exit(exit_s);
	(void)exit_code;
}
