/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 12:35:12 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/02 18:09:27 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned int	i;
	unsigned char	*str;
	unsigned char	*ptr;

	str = (unsigned char *)s1;
	ptr = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (str[i] != ptr[i])
			return (str[i] - ptr[i]);
		i++;
	}
	return (0);
}
