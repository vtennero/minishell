/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd_ulong.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:48:28 by vitenner          #+#    #+#             */
/*   Updated: 2023/10/27 19:18:51 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd_ulong(unsigned long long nb, int fd)
{
	if (nb >= 10)
	{
		ft_putnbr_fd_ulong(nb / 10, fd);
		ft_putnbr_fd_ulong(nb % 10, fd);
	}
	else
		ft_putchar_fd(nb + 48, fd);
}
