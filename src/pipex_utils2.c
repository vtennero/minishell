/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:24:55 by cliew             #+#    #+#             */
/*   Updated: 2024/04/10 18:25:06 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup_ignore(const char *s, char ignore)
{
	char	*dup;
	size_t	slen;
	size_t	i;

	slen = ft_strlen(s);
	i = ft_strchr_count(s, ignore);
	dup = (char *)malloc((slen - i + 1) * sizeof(char));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != ignore)
		{
			dup[i] = *s;
			i++;
		}
		s++;
	}
	dup[i] = 0;
	return (dup);
}

int	ft_puterr(char *s, int ret)
{
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
	exit (ret);
}

int	find_env_var(t_env_var *list, const char *key)
{
	while (list != NULL)
	{
		if (ft_strcmp(list->key, (char *)key) == 0)
			return (1);
		list = list->next;
	}
	return (0);
}
