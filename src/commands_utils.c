/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:16:31 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 14:16:46 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
