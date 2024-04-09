/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_set_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:13:35 by root              #+#    #+#             */
/*   Updated: 2024/04/09 18:57:36 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_only_spaces(char *str)
{
	int	length;
	int	i;

	length = ft_strlen(str);
	if ((str[0] == '\'' && str[length - 1] == '\'') || (str[0] == '"'
			&& str[length - 1] == '"'))
	{
		i = 1;
		while (i < length - 1)
		{
			if (!isspace(str[i]))
			{
				return (0);
			}
			i++;
		}
		return (1);
	}
	return (0);
}
