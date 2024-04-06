/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:55:49 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/06 19:02:50 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *process_single_quote(const char **s)
{

	(*s)++;
	const char *start = *s;
	while (**s && **s != '\'')
		(*s)++;
	int len = *s - start;
	char *result = strndup(start, len);
	(*s)++;
	return (result);
}

char *process_double_quote(const char **s, t_shell *shell)
{
	char *buf = NULL;
	char	*newBuf;
	size_t textLen;

	(*s)++;
	const char *endQuote = strchr(*s, '\"');

	if (!endQuote)
	{
		buf = malloc(2);
		if (buf) {
			buf[0] = **s;
			buf[1] = '\0';
		}
		(*s)++;
		return buf;
	}
	while (*s < endQuote)
	{
		if (**s == '$') {
			char *expanded = expandVariables(shell, *s);
			if (!buf)
				newBuf = expanded;
			else
				newBuf = ft_strjoin(buf, expanded);
			buf = newBuf;
			textLen = get_non_expanded_var_length((char * )(*s));
			*s += textLen;
		} else {
			const char *nextDollar = strchr(*s, '$');
			if (!nextDollar || nextDollar > endQuote) {
				nextDollar = endQuote; // If no $ is found, copy until the end quote
			}
			textLen = nextDollar - *s;
			char *duplicatedText = strndup(*s, textLen);
			if (!buf)
				newBuf = duplicatedText;
			else
				newBuf = ft_strjoin(buf, duplicatedText);
			buf = newBuf;
			*s += textLen;
		}
	}
	if (**s == '\"') (*s)++;
	return buf;
}
