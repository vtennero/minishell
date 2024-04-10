/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:55:49 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 19:52:38 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_single_quote(t_shell *shell, const char **s)
{
	char		*result;
	const char	*start;
	int			len;

	(*s)++;
	start = *s;
	while (**s && **s != '\'')
		(*s)++;
	len = *s - start;
	result = shell_strndup(shell, start, len);
	(*s)++;
	return (result);
}

char	*create_one_char_str(t_shell *shell, char c)
{
	char	*temp;

	temp = shell_malloc(shell, 2);
	temp[0] = c;
	temp[1] = '\0';
	return (temp);
}

char	*process_quoting(t_shell *shell, const char **s, char *result)
{
	size_t	adv_position;
	char	*temp;

	while (**s && !ft_isspace((unsigned char)**s) && !(is_special(*s)))
	{
		temp = NULL;
		if (**s == '\'')
			temp = process_single_quote(shell, s);
		else if (**s == '\"')
			temp = process_double_quote(s, shell);
		else if (**s == '$')
		{
			temp = expand_var_two(shell, *s, &adv_position);
			*s += adv_position;
		}
		else
		{
			temp = create_one_char_str(shell, **s);
			(*s)++;
		}
		if (temp)
			result = shell_strjoin(shell, result, temp);
	}
	return (result);
}
