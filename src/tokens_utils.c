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

int	is_special(const char *str)
{
	if (ft_strncmp(str, "|", 1) == 0)
		return (1);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (2);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (2);
	else if (ft_strncmp(str, ">", 1) == 0)
		return (1);
	else if (ft_strncmp(str, "<", 1) == 0)
		return (1);
	return (0);
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

int	get_token_type(const char *token_text)
{
	if (ft_strcmp(token_text, "<") == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strcmp(token_text, ">") == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strcmp(token_text, ">>") == 0)
		return (TOKEN_REDIR_APPEND);
	else if (ft_strcmp(token_text, "<<") == 0)
		return (TOKEN_REDIR_HEREDOC);
	else if (ft_strcmp(token_text, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(token_text, "$?") == 0)
		return (TOKEN_EXIT_STATUS);
	return (TOKEN_ARG);
}
