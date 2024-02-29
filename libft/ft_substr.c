/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:48:53 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/09 14:55:44 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*new_string;
	size_t	string_length;
	size_t	finish;

	if (!s)
		return (0);
	string_length = ft_strlen(s);
	if (start >= string_length)
	{
		new_string = (char *)malloc(1);
		if (!new_string)
			return (0);
		new_string[0] = '\0';
		return (new_string);
	}
	finish = 0;
	if (start < string_length)
		finish = string_length - start;
	if (finish > len)
		finish = len;
	new_string = (char *)malloc(sizeof(char) * (finish + 1));
	if (!new_string)
		return (0);
	ft_strlcpy(new_string, s + start, finish + 1);
	return (new_string);
}
