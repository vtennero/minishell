/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:38:52 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/14 11:05:28 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	not_empty(const char *str)
{
	size_t	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] != '\0')
	{
		if (!ft_isspace((unsigned char)str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	end_with_pipe(const char *str)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i] != '\0')
		i++;
	while (i > 0 && ft_isspace((unsigned char)str[i - 1]))
		i--;
	if (i > 0 && str[i - 1] == '|')
		return (1);
	else
		return (0);
}

int	check_tokens( t_token_node *node)
{
	t_token_node	*t;
	int				et;

	t = node;
	while (t && t != NULL)
	{
		if (t->token.type == 6 && t->next && t->next->token.type == 6)
			return (1);
		if (is_redirect(t->token.type, &et) && t->next && \
		is_redirect(t->next ->token.type, &et))
			return (1);
		if (is_redirect(t->token.type, &et) && !t->next)
			return (1);
		t = t->next;
	}
	return (0);
}
