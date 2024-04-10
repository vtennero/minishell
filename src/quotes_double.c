/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_double.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:09:19 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 14:19:57 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*handle_dol_in_quote(const char **s, t_shell *shell, char *buf)
{
	char	*new;
	size_t	text_len;
	char	*expanded;

	expanded = expand_var_one(shell, *s);
	text_len = get_non_expanded_var_length((char *)(*s));
	if (!buf)
		new = expanded;
	else
	{
		new = shell_strjoin(shell, buf, expanded);
		free(buf);
	}
	*s += text_len;
	return (new);
}

char	*norm_char_quote(const char **s, t_shell *shell, const char *e, char *b)
{
	char		*new;
	const char	*next_dol;
	size_t		text_len;
	char		*duplicated_text;

	next_dol = ft_strchr(*s, '$');
	if (!next_dol || next_dol > e)
		next_dol = e;
	text_len = next_dol - *s;
	duplicated_text = shell_strndup(shell, *s, text_len);
	if (!b)
		new = duplicated_text;
	else
		new = shell_strjoin(shell, b, duplicated_text);
	*s += text_len;
	return (new);
}

char	*handle_w_e_quote(const char **s, t_shell *shell, const char *e_quote)
{
	char	*buf;

	buf = NULL;
	while (*s < e_quote)
	{
		if (**s == '$')
			buf = handle_dol_in_quote(s, shell, buf);
		else
			buf = norm_char_quote(s, shell, e_quote, buf);
	}
	if (**s == '\"')
		(*s)++;
	return (buf);
}

char	*handle_no_e_quote(t_shell *shell, const char **s)
{
	char	*buf;

	buf = shell_malloc(shell, 2);
	if (buf)
	{
		buf[0] = **s;
		buf[1] = '\0';
	}
	(*s)++;
	return (buf);
}

char	*process_double_quote(const char **s, t_shell *shell)
{
	char		*result;
	const char	*e_quote;

	(*s)++;
	e_quote = ft_strchr(*s, '\"');
	if (!e_quote)
		result = handle_no_e_quote(shell, s);
	else
		result = handle_w_e_quote(s, shell, e_quote);
	return (result);
}
