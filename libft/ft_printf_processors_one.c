/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_processors_one.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:40:59 by vitenner          #+#    #+#             */
/*   Updated: 2023/12/13 10:57:39 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_s(va_list arg)
{
	char	*str;

	str = va_arg(arg, char *);
	if (str)
	{
		ft_putstr(str);
		return (ft_strlen(str));
	}
	ft_putstr("(null)");
	return (6);
}

int	ft_is_p(va_list arg)
{
	void	*ptr;

	ptr = va_arg(arg, void *);
	return (ft_build_address((unsigned long long)ptr));
}

int	ft_is_d(va_list arg)
{
	int				num;
	int				len;
	char			*str;

	num = va_arg(arg, int);
	ft_putnbr_fd(num, 1);
	str = ft_itoa(num);
	len = ft_strlen(str);
	free(str);
	return (len);
}

int	ft_is_i(va_list arg)
{
	int				num;
	int				len;
	char			*str;

	num = va_arg(arg, int);
	ft_putnbr_fd(num, 1);
	str = ft_itoa(num);
	len = ft_strlen(str);
	free(str);
	return (len);
}
