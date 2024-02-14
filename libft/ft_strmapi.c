/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:48:47 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/02 18:12:18 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	int		l;
	int		i;

	i = 0;
	str = NULL;
	if (s && f)
	{
		l = ft_strlen(s);
		str = malloc(sizeof(char) * l + 1);
		if (!str || !s || !f)
			return (NULL);
		while (i < l)
		{
			str[i] = f(i, s[i]);
			i++;
		}
		str[i] = '\0';
	}
	return (str);
}
