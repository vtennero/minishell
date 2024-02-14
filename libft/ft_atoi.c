/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 12:34:12 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/02 18:06:47 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int		number;
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
