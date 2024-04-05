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
	char	*new;
	size_t	len;

	len = ft_strlen(s) + 1;
	new = (char *)shell_malloc(shell, len);
	if (new)
	{
		ft_strncpy(new, (char *)s, len);
		new[len - 1] = '\0';
	}
	return (new);
}

char	*shell_strndup(t_shell *shell, const char *s, size_t n)
{
	size_t	len;
	char	*new;

	len = strlen(s);
	if (len > n)
		len = n;
	new = (char *)shell_malloc(shell, len + 1);
	if (new)
	{
		ft_strncpy(new, s, len);
		new[len] = '\0';
	}
	return (new);
}

char	*shell_itoa(t_shell *shell, int n)
{
	int		neg;
	int		i;
	char	*str;

	neg = 0;
	if (n < 0)
		neg = 1;
	i = ft_intlen((n));
	str = shell_malloc(shell, sizeof(char) * i + 1);
	str[i] = '\0';
	if (str)
	{
		while (i > 0)
		{
			str[--i] = ft_abs(n % 10) + 48;
			n /= 10;
		}
		if (neg == 1)
			str[0] = '-';
	}
	return (str);
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
