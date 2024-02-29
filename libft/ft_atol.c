/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 12:44:53 by vitenner          #+#    #+#             */
/*   Updated: 2024/01/10 12:45:42 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *str)
{
	long	number;
	int		sign;
	char	*ptr;

	ptr = (char *)str;
	sign = 1;
	number = 0;
	while ((*ptr >= 9 && *ptr <= 13) || *ptr == 32)
		ptr++;
	if (*ptr == '+' )
		ptr++;
	else if (*ptr == '-')
	{
		sign = -1;
		ptr++;
	}
	while (*ptr >= 48 && *ptr <= 57)
	{
		number = number * 10;
		number = number + (*ptr - 48);
		ptr++;
	}
	return (sign * number);
}
