/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:55:49 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 11:10:42 by vitenner         ###   ########.fr       */
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

char	*norm_char_quote(const char **s, t_shell *shell, const char *e_quote, char *buf)
{
	char		*new;
	const char	*next_dol;
	size_t		text_len;
	char		*duplicated_text;

	next_dol = ft_strchr(*s, '$');
	if (!next_dol || next_dol > e_quote)
		next_dol = e_quote;
	text_len = next_dol - *s;
	duplicated_text = shell_strndup(shell, *s, text_len);
	if (!buf)
		new = duplicated_text;
	else
		new = shell_strjoin(shell, buf, duplicated_text);
	*s += text_len;
	return (new);
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

	ft_printf("process_quoting START result = |%s|\n", result);
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
		ft_printf("process_quoting temp = |%s|\n", temp);
		if (temp)
			result = shell_strjoin(shell, result, temp);
	}
	ft_printf("process_quoting result = |%s|\n", result);
	return (result);
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
