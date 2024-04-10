/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:57:46 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 10:57:46 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*skip_delimiters(const char *s, char c)
{
	while (*s == ' ' || *s == '\f' || *s == '\r' || *s == '\t' || *s == '\v'
		|| *s == '\n')
		s++;
	(void)c;
	return (s);
}

int	find_index_char(const char *str, char c)
{
	const char	*found;

	found = ft_strchr(str, c);
	if (found != NULL)
	{
		return (found - str);
	}
	return (-1);
}

int	get_token_list_length(TokenNode *head)
{
	int			length;
	TokenNode	*current;

	length = 0;
	current = head;
	while (current != NULL)
	{
		length++;
		current = current->next;
	}
	return (length);
}

int	calc_int_len(int num)
{
	int	length;

	if (num == 0)
		return (1);
	length = 0;
	while (num != 0)
	{
		length++;
		num /= 10;
	}
	return (length);
}

int	isspace_not_eol(int c)
{
	return (c == ' ' || c == '\f' || c == '\r' || c == '\t' || c == '\v');
}
