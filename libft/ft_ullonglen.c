/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ullonglen.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 18:56:09 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/30 12:30:46 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_ullonglen(unsigned long long n)
{
	int	len;

	len = 1;
	while (n / 10)
	{
		n /= 10;
		++len;
	}
	return (len);
}
