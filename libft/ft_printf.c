/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:15:29 by vitenner          #+#    #+#             */
/*   Updated: 2023/12/13 10:56:30 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_struct	g_formats[127];

void	ft_set_g_formats(void)
{
	g_formats['s'].printfunc = ft_is_s;
	g_formats['p'].printfunc = ft_is_p;
	g_formats['d'].printfunc = ft_is_d;
	g_formats['i'].printfunc = ft_is_i;
	g_formats['u'].printfunc = ft_is_u;
	g_formats['x'].printfunc = ft_is_x;
	g_formats['X'].printfunc = ft_is_cap_x;
	g_formats['c'].printfunc = ft_is_c;
	g_formats['%'].printfunc = ft_is_percentage;
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	ft_set_g_formats();
	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format >= 0 && g_formats[(unsigned char)(*format)].printfunc)
				count += g_formats[(unsigned char)(*format)].printfunc(args);
		}
		else
			count += ft_putchar_count(*format);
		format++;
	}
	return (count);
}
