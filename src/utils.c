/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:05:36 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/31 21:49:03 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*skip_delimiters(const char *s, char c)
{
	// while (*s == c)
	// s++;
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

int	getTokenListLength(TokenNode *head)
{
	int			length;
	TokenNode	*current;

	length = 0;
	current = head;
	while (current != NULL)
	{
		length++;
		current = current->next; // Move to the next node
	}
	return (length);
}

int	intLength(int num)
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
