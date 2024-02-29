/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd_longlong.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:48:28 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/27 19:10:47 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd_longlong(long long nb, int fd)
{
	if (nb == -9223372036854775807LL - 1)
	{
		ft_putchar_fd(45, fd);
		ft_putnbr_fd_longlong(9223372036, fd);
		ft_putnbr_fd_longlong(854775808, fd);
	}
	else
	{
		if (nb < 0 && nb > -9223372036854775807LL - 1)
		{
			ft_putchar_fd(45, fd);
			ft_putnbr_fd_longlong(-nb, fd);
		}
		if (nb >= 10)
		{
			ft_putnbr_fd_longlong(nb / 10, fd);
			ft_putnbr_fd_longlong(nb % 10, fd);
		}
		if (nb < 10 && nb >= 0)
			ft_putchar_fd(nb + 48, fd);
	}
}
