/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 12:35:21 by vitenner          #+#    #+#             */
/*   Updated: 2023/12/13 11:03:51 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		n;
	char	*cpy;
	char	a;

	n = 0;
	if (!s)
		return (NULL);
	cpy = (char *)s;
	a = (char)c;
	while (cpy[n] != '\0')
	{
		if (cpy[n] == a)
			return (&cpy[n]);
		n++;
	}
	if (cpy[n] == a)
		return (&cpy[n]);
	else
		return (NULL);
}
