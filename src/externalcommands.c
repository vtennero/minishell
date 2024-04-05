/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externalcommands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:31:07 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/18 14:32:07 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_env_var(t_env_var *list, const char *key)
{
	while (list != NULL)
	{
		if (ft_strcmp(list->key, (char *)key) == 0)
			return (1);
		list = list->next;
	}
	return (0);
}
