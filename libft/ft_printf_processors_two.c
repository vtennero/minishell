/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_processors_two.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:40:02 by vitenner          #+#    #+#             */
/*   Updated: 2023/12/13 10:57:32 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_u(va_list arg)
{
	unsigned int	unum;
	int				len;
	char			*str;

	unum = va_arg(arg, unsigned int);
	ft_putnbr_fd_ulong(unum, 1);
	str = ft_lutoa(unum);
	len = ft_strlen(str);
	free(str);
	return (len);
}

int	ft_is_x(va_list arg)
{
	unsigned int	hex_num;
	int				len;

	hex_num = va_arg(arg, unsigned int);
	len = ft_hex_length(hex_num);
	ft_putuintnbr_base(hex_num, "0123456789abcdef");
	return (len);
}

int	ft_is_cap_x(va_list arg)
{
	unsigned int	hex_num_upper;
	int				len;

	hex_num_upper = va_arg(arg, unsigned int);
	len = ft_hex_length(hex_num_upper);
	ft_putuintnbr_base(hex_num_upper, "0123456789ABCDEF");
	return (len);
}

int	ft_is_c(va_list arg)
{
	char	c;

	c = va_arg(arg, int);
	ft_putchar_fd(c, 1);
	return (1);
}

int	ft_is_percentage(va_list arg)
{
	char	c;

	c = '%';
	(void)arg;
	ft_putchar_fd(c, 1);
	return (1);
}
