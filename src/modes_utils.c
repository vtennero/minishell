/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:38:52 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/08 22:10:44 by cliew            ###   ########.fr       */
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
