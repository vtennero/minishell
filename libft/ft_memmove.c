/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 12:35:16 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/02 18:09:54 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*s1;
	unsigned char	*s2;

	i = 0;
	s1 = (unsigned char *)dst;
	s2 = (unsigned char *)src;
	if (len && (s1 || s2))
	{
		if (dst <= src)
			i = 0;
		else
			i = len - 1;
		while (len-- > 0)
		{
			s1[i] = s2[i];
			if (dst > src)
				i--;
			else
				i++;
		}
	}
	return (s1);
}
