/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:24:32 by toto              #+#    #+#             */
/*   Updated: 2024/02/26 15:48:04 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int isSpecialOperator(const char *str) {
    if (strncmp(str, "|", 1) == 0) {
        return 1;
    } else if (strncmp(str, "<<", 2) == 0) {
        return 2;
    } else if (strncmp(str, ">>", 2) == 0) {
        return 2;
    } else if (strncmp(str, ">", 1) == 0) {
        return 1;
    } else if (strncmp(str, "<", 1) == 0) {
        return 1;
    }
    return 0;
}

int	get_non_expanded_var_length(char *var)
{
    int count = 1; // Counter for valid characters
    int index = 0; // Current index in the string
    int foundLetter = 0; // Flag to track if at least one letter has been found
    int foundDol = 1; // Flag to track if at least one letter has been found

	var++;
    while(var[index] != '\0')
	{
		if (var[index] == '$')
			foundDol++;
        else if(!ft_isalpha(var[index]) && !ft_isdigit(var[index]) && var[index] != '?') {
            break;
        }
		else if (var[index] == '?' && var[index + 1] == '\"')
			return(2);
        else if(ft_isalpha(var[index])) {
            foundLetter = 1; // Set the flag if a letter is found
        }
        count++; // Increment the count of valid characters
        index++; // Move to the next character
    }
	if (foundDol == count)
		return (count);
	else if (!foundLetter)
		count = 1;
	return (count);
}

char *quotevar(t_shell *shell, const char **s) {
    char *result = shell_strdup(shell,""); // Start with an empty string

	if (!isSpecialOperator(*s))
	{
		while (**s && !isspace((unsigned char)**s) && !(isSpecialOperator(*s)))
		{
			char *temp = NULL;
			int opLength = isSpecialOperator((*s));
			if (**s == '\'') {
				temp = process_single_quote(s);
			} else if (**s == '\"') {
				temp = process_double_quote(s, shell);
			} else if (**s == '$') {
				size_t advancedPosition = 0;
				temp = expandVariables2(shell, *s, &advancedPosition);
				*s += advancedPosition; // Adjust for loop increment
			}
			else {
				temp = shell_malloc(shell,2); // Allocate space for char and null terminator
				temp[0] = **s;
				temp[1] = '\0';
				(*s)++;
			}
			(void)opLength;
			if (temp) {
				char *new_result = ft_strjoin(result, temp);
				result = shell_strdup(shell,new_result); // Update the result with the new concatenated string
				free(new_result);
			}
		}
	}
	else if (isSpecialOperator(*s))
	{
		result = shell_strndup(shell,(*s), isSpecialOperator(*s));
		*s += isSpecialOperator(*s);
	}
    return result; // This is the concatenated final result
}


char    *parse_tokens(t_shell *shell, const char *s)
{
	char    *wvarexpanded;
	int     type;
	int		index;

	index = 0;
	while (*s)
	{
		s = skip_delimiters(s, ' ');
		if (!*s)
			break;
		wvarexpanded = quotevar(shell, &s);
		if (index == 0)
			type = TOKEN_COMMAND;
		else
			type = get_token_type(wvarexpanded);
		addToken(shell, wvarexpanded, type);
		index++;
		s = skip_delimiters(s, ' ');
	}
	return (NULL);
}


void addToken(t_shell *shell, const char *value, int type)
{
	TokenNode *newNode = (TokenNode *)shell_malloc(shell, sizeof(TokenNode));
	if (!newNode)
		return;

	newNode->token.value = shell_strdup(shell, value); // Use shell_strdup
	newNode->token.type = type;
	newNode->next = NULL;
	if (shell->token_head == NULL)
		shell->token_head = newNode;
	else
	{
		TokenNode *current = shell->token_head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newNode;
	}
}

TokenType get_token_type(const char* token_text)
{
	if (ft_strcmp(token_text, "<") == 0)
		return TOKEN_REDIR_IN;
	else if (ft_strcmp(token_text, ">") == 0)
		return TOKEN_REDIR_OUT;
	else if (ft_strcmp(token_text, ">>") == 0)
		return TOKEN_REDIR_APPEND;
	else if (ft_strcmp(token_text, "<<") == 0)
		return TOKEN_REDIR_HEREDOC;
	else if (ft_strcmp(token_text, "|") == 0)
		return TOKEN_PIPE;
	else if (ft_strcmp(token_text, "$?") == 0)
		return TOKEN_EXIT_STATUS;
	return TOKEN_ARG;
}

void create_tokens(t_shell *shell, const char *s)
{
	parse_tokens(shell, s);
	set_token_commands(shell);
}
