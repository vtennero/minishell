/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:55:49 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/06 20:26:23 by vitenner         ###   ########.fr       */
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

char *handleNoEndQuote(t_shell *shell, const char **s)
{
    char *buf = shell_malloc(shell, 2); // Allocate memory for 2 characters (1 character + '\0')
    if (buf) {
        buf[0] = **s; // Assign the current character to the first position of buf
        buf[1] = '\0'; // Null-terminate the string
    }
    (*s)++; // Advance the pointer to the next character
    return (buf); // Return the created buffer
}

char *handleDollarInQuote(const char **s, t_shell *shell, char *buf) {
    char *newBuf;
    size_t textLen = get_non_expanded_var_length((char *)(*s));
    char *expanded = expandVariables(shell, *s);

    if (!buf)
        newBuf = expanded;
    else {
        newBuf = shell_strjoin(shell, buf, expanded);
        free(buf); // Assuming shell_strjoin allocates a new buffer
    }

    *s += textLen;
    return newBuf;
}

char *handleOtherInQuote(const char **s, t_shell *shell, const char *endQuote, char *buf) {
    char *newBuf;
    const char *nextDollar = ft_strchr(*s, '$');
    if (!nextDollar || nextDollar > endQuote) {
        nextDollar = endQuote; // If no $ is found, copy until the end quote
    }
    size_t textLen = nextDollar - *s;
    char *duplicatedText = shell_strndup(shell, *s, textLen);

    if (!buf)
        newBuf = duplicatedText;
    else {
        newBuf = shell_strjoin(shell, buf, duplicatedText);
        free(buf); // Assuming shell_strjoin allocates a new buffer
    }

    *s += textLen;
    return newBuf;
}


// char *handleWithEndQuote(const char **s, t_shell *shell, const char *endQuote) {
//     char *buf = NULL;
//     char *newBuf;
//     size_t textLen;

//     while (*s < endQuote) {
//         if (**s == '$') {
//             char *expanded = expandVariables(shell, *s);
//             if (!buf)
//                 newBuf = expanded;
//             else {
//                 newBuf = shell_strjoin(shell, buf, expanded);
//                 free(buf); // Free the old buffer to prevent memory leaks
//             }
//             buf = newBuf;
//             textLen = get_non_expanded_var_length((char *)(*s));
//             *s += textLen;
//         } else {
//             const char *nextDollar = ft_strchr(*s, '$');
//             if (!nextDollar || nextDollar > endQuote) {
//                 nextDollar = endQuote; // If no $ is found, copy until the end quote
//             }
//             textLen = nextDollar - *s;
//             char *duplicatedText = shell_strndup(shell, *s, textLen);
//             if (!buf)
//                 newBuf = duplicatedText;
//             else {
//                 newBuf = shell_strjoin(shell, buf, duplicatedText);
//                 free(buf); // Free the old buffer
//             }
//             buf = newBuf;
//             *s += textLen;
//         }
//     }
//     if (**s == '\"')
//         (*s)++;
//     return buf;
// }

char *handleWithEndQuote(const char **s, t_shell *shell, const char *endQuote) {
    char *buf = NULL;

    while (*s < endQuote) {
        if (**s == '$') {
            buf = handleDollarInQuote(s, shell, buf);
        } else {
            buf = handleOtherInQuote(s, shell, endQuote, buf);
        }
    }
    if (**s == '\"')
        (*s)++;
    return buf;
}




char *process_double_quote(const char **s, t_shell *shell)
{
    char *result;
    (*s)++; // Skip the initial double quote
    const char *endQuote = ft_strchr(*s, '\"');

    if (!endQuote)
        result = handleNoEndQuote(shell, s);
    else
        result = handleWithEndQuote(s, shell, endQuote);
    return (result);
}
