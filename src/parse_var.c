/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:14:21 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 14:15:07 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isvalidvarstartchar(char c)
{
	return (ft_isalpha(c) || (c == '_'));
}

int	isvalidvarchar(char c)
{
	return (ft_isalnum(c) || (c == '_'));
}

int	check_first_var_char(char *var, int *baselen)
{
	if (var[1] == '\"')
	{
		*baselen = 2;
		return (0);
	}
	else if (!var[1] || isspace_not_eol(var[1] || var[1] == '\"'
			|| var[1] == '$'))
		return (0);
	else if (var[1] == '?')
	{
		*baselen = 2;
		return (0);
	}
	else if (isvalidvarstartchar(var[1]))
		(*baselen)++;
	return (1);
}

int	check_susbsequent_chars(char *var, int *baselen)
{
	int	index;

	index = 1;
	while (var[index])
	{
		if (isvalidvarchar(var[index]))
			(*baselen)++;
		else if (var[index] == '?')
		{
			(*baselen)++;
			break ;
		}
		else
			break ;
		index++;
	}
	return (0);
}

int	get_non_expanded_var_length(char *var)
{
	int	index;
	int	baselen;
	int	result;

	index = 1;
	baselen = 1;
	result = check_first_var_char(var, &baselen);
	if (result == 0)
		return (baselen);
	index++;
	result = check_susbsequent_chars(var, &baselen);
	return (baselen);
}
