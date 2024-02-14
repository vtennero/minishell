/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 13:44:43 by vitenner          #+#    #+#             */
/*   Updated: 2023/12/13 11:04:39 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc_set_zero(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	i;
	char	*str;

	ptr = (void *)malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	str = (char *)ptr;
	i = 0;
	while (i < nmemb * size)
		str[i++] = 0;
	return (str);
}
