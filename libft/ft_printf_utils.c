/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:32:01 by vitenner          #+#    #+#             */
/*   Updated: 2023/12/13 10:57:26 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_hex_length(unsigned int num)
{
	int	length;

	if (num == 0)
		return (1);
	length = 0;
	while (num)
	{
		length++;
		num /= 16;
	}
	return (length);
}

char	*ft_uint_to_hex(unsigned int num, int caps)
{
	char	hex_chars[17];
	char	hex_chars_cap[17];
	char	*hex_str;
	int		len;

	ft_strlcpy(hex_chars, "0123456789abcdef", 17);
	ft_strlcpy(hex_chars_cap, "0123456789ABCDEF", 17);
	len = ft_hex_length(num);
	hex_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!hex_str)
		return (NULL);
	hex_str[len--] = '\0';
	while (len >= 0)
	{
		if (caps == 1)
			hex_str[len] = hex_chars_cap[num % 16];
		else
			hex_str[len] = hex_chars[num % 16];
		num /= 16;
		len--;
		ft_putstr(hex_str);
	}
	return (hex_str);
}

static int	ft_print_address(char *address, int count)
{
	int		sub_counter;

	sub_counter = 2;
	write(1, "0x", 2);
	while (--count >= 0)
	{
		write(1, &(address[count]), 1);
		sub_counter++;
	}
	return (sub_counter);
}

int	ft_build_address(unsigned long long num)
{
	char	hex_digits[17];
	char	buffer[17];
	int		idx;
	int		counter;

	idx = 0;
	counter = 0;
	ft_strlcpy(hex_digits, "0123456789abcdef", 17);
	if (num != 0)
	{
		while (num > 0)
		{
			buffer[idx++] = hex_digits[num % 16];
			num /= 16;
		}
		counter += ft_print_address(buffer, idx);
	}
	else
	{
		write(1, "(nil)", 5);
		counter += 5;
	}
	return (counter);
}

int	ft_putchar_count(char c)
{
	write(1, &c, 1);
	return (1);
}
