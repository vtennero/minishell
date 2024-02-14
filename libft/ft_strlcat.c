/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 12:35:24 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/02 18:13:16 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	ldst;
	size_t	lsrc;

	i = 0;
	ldst = ft_strlen(dst);
	lsrc = ft_strlen((char *)src);
	if (size > (ldst + 1))
	{
		while (i < (size - ldst - 1) && (i < lsrc))
		{
			dst[ldst + i] = src[i];
			i++;
		}
		dst[ldst + i] = '\0';
	}
	if (size >= ldst)
		return (ldst + lsrc);
	return (lsrc + size);
}
