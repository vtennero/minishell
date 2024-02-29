/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:28:21 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/23 10:49:10 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *litl, size_t len)
{
	size_t	h;
	size_t	n;

	h = 0;
	if (litl[0] == '\0')
		return ((char *)big);
	while (big[h] != '\0')
	{
		n = 0;
		while (big[h + n] == litl[n] && (h + n) < len)
		{
			if (big[h + n] == '\0' && litl[n] == '\0')
				return ((char *)&big[h]);
			n++;
		}
		if (litl[n] == '\0')
			return ((char *)&big[h]);
		h++;
	}
	return (0);
}
