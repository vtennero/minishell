/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 12:35:44 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/06 16:05:56 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*pos;
	char	a;

	a = (char)(c);
	pos = (char *)s + ft_strlen(s);
	while (pos >= s)
	{
		if (*pos == a)
			return (pos);
		--pos;
	}
	if (a == '\0')
		return (pos +1);
	return (NULL);
}
