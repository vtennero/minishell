/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:48:28 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/27 16:39:27 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int nb, int fd)
{
	if (nb == -2147483648)
	{
		ft_putchar_fd(45, fd);
		ft_putnbr_fd(21474, fd);
		ft_putnbr_fd(83648, fd);
	}
	else
	{
		if (nb < 0 && nb > -2147483648)
		{
			ft_putchar_fd(45, fd);
			ft_putnbr_fd(-nb, fd);
		}
		if (nb >= 10)
		{
			ft_putnbr_fd(nb / 10, fd);
			ft_putnbr_fd(nb % 10, fd);
		}
		if (nb < 10 && nb >= 0)
			ft_putchar_fd(nb + 48, fd);
	}
}
