/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:48:34 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/09 14:59:37 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strsplit_strdup(const char *src, char c)
{
	char	*wd;
	int		l;
	int		i;

	l = 0;
	while (src[l] && src[l] != c)
		l++;
	wd = malloc(l * sizeof(char) + 1);
	if (!wd)
		return (NULL);
	i = 0;
	while (i < l)
	{
		wd[i] = src[i];
		i++;
	}
	wd[i] = '\0';
	return (wd);
}

static int	ft_strsplit_nbwds(const char *str, char c)
{
	int	k;
	int	i;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
			k++;
		i++;
	}
	return (k);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		n;
	int		i;
	char	*str;

	if (!s)
		return (NULL);
	str = (char *)s;
	i = 0;
	n = ft_strsplit_nbwds(str, c);
	tab = malloc(sizeof(char *) * (n + 1));
	if (!tab)
		return (NULL);
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str && *str != c)
			tab[i++] = ft_strsplit_strdup(str, c);
		while (*str && *str != c)
			str++;
	}
	tab[i] = 0;
	return (tab);
}
