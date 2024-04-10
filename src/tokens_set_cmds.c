/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_set_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:13:35 by root              #+#    #+#             */
/*   Updated: 2024/04/10 10:46:02 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_only_spaces(char *str)
{
	int	length;

	length = ft_strlen(str);
	if ((str[0] == '\'' && str[length - 1] == '\'') || (str[0] == '"' && str[length - 1] == '"')) {
		int i = 1;

		while (i < length - 1) {
			if (!ft_isspace(str[i])) {
				return 0;
			}
			i++;
		}

		return 1;
	}

	return 0;
}

