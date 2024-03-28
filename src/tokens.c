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

// v4


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
    return 0; // Not a match
}



// v3 tokenizer

int isspace_not_eol(int ch)
{
    return (ch == ' ' || ch == '\f' || ch == '\r' || ch == '\t' || ch == '\v');
}

char *processSingleQuote(const char **s)
{
	// ft_printf("processSingleQuote start s |%s|\n", *s);
	(*s)++;
    const char *start = *s; // Skip the initial quote
    while (**s && **s != '\'')
		(*s)++;
	// ft_printf("processSingleQuote s after while |%s|\n", *s);
	int len = *s - start;
	// ft_printf("processSingleQuote len |%d|\n", len);
	char *result = strndup(start, len);
	// ft_printf("processSingleQuote s before going past word |%s|\n", *s);
    // if (**s == '\'') (*s)++;
	// (*s) += len + 1;
	// ft_printf("processSingleQuote s after going past word |%s|\n", *s);
	// ft_printf("processSingleQuote result |%s|\n", result);
    return result;
}


int	get_non_expanded_var_length(char *var)
{
    int count = 1; // Counter for valid characters
    int index = 0; // Current index in the string
    int foundLetter = 0; // Flag to track if at least one letter has been found
    int foundDol = 1; // Flag to track if at least one letter has been found

	// ft_printf("get_non_expanded_var_length %s\n", var);
	var++;
	// ft_printf("get_non_expanded_var_length %s\n", var);
    while(var[index] != '\0') {
		if (var[index] == '$')
			foundDol++;
        else if(!isalpha(var[index]) && !isdigit(var[index])) {
            // If the character is not a letter or digit, break the loop
            break;
        }
        else if(isalpha(var[index])) {
            foundLetter = 1; // Set the flag if a letter is found
        }
        count++; // Increment the count of valid characters
        index++; // Move to the next character
    }
	// ft_printf("get_non_expanded_var_length found Dol %d count %d\n", foundDol, count);
	if (foundDol == count)
		return (count);
	else if (!foundLetter)
		count = 1;
		// count = 0;
	return (count);
}



char *processDoubleQuote(const char **s, t_shell *shell) {
    char *buf = NULL;
	char	*newBuf;
	size_t textLen;

    // 1. Find the closing quote
	(*s)++;
    const char *endQuote = strchr(*s, '\"');
    
    // a. If a closing quote is not found
    if (!endQuote) {
        buf = malloc(2); // for the quote and the null terminator
        if (buf) {
            buf[0] = **s; // Copy the opening quote
            buf[1] = '\0'; // Null-terminate the string
        }
        (*s)++; // Move past the opening quote
        return buf;
    }

    // 2. Iterate until the closing quote is reached
    while (*s < endQuote) {
		// ft_printf("processDoubleQuote while (*s < endQuote)  |%s|\n", *s);
        if (**s == '$') {
            // a. Expand the variable
            char *expanded = expandVariables(shell, *s);
			// ft_printf("processDoubleQuote char *expanded = expandVariables(shell, *s); |%s|\n", expanded);
			if (!buf)
				newBuf = expanded;
			else
            	newBuf = ft_strjoin(buf, expanded);
            // free(buf); // Free the old buffer
            // free(expanded); // Free the expanded string
            buf = newBuf;
			// ft_printf("processDoubleQuote buf |%s|\n", buf);
			textLen = get_non_expanded_var_length((char * )(*s));
			// ft_printf("processDoubleQuote get_non_expanded_var_length |%d|\n", textLen);
			// ft_printf("processDoubleQuote s |%s|\n", *s);
            *s += textLen;
			// ft_printf("processDoubleQuote s |%s|\n", *s);
            // Move pointer s by n characters, where n is the length of the variable
            // Assuming expandVariables moves *s to the end of the variable
        } else {
			// ft_printf("processDoubleQuote while (*s < endQuote) else not $ |%s|\n", *s);
            // b. Duplicate text until the next $ or the end quote
            const char *nextDollar = strchr(*s, '$');
            if (!nextDollar || nextDollar > endQuote) {
                nextDollar = endQuote; // If no $ is found, copy until the end quote
            }
            textLen = nextDollar - *s;
            char *duplicatedText = strndup(*s, textLen);
			// ft_printf("processDoubleQuote duplicatedText |%s|\n", duplicatedText);
			if (!buf)
				newBuf = duplicatedText;
			else
            	newBuf = ft_strjoin(buf, duplicatedText);
            // free(buf); // Free the old buffer
            // free(duplicatedText); // Free the duplicated text
            buf = newBuf;
			// ft_printf("processDoubleQuote else buf |%s|\n", buf);

            // Move the pointer to the position of the next $ or end quote
            *s += textLen;
        }
    }

    // Move past the closing quote for the next iteration
    if (**s == '\"') (*s)++;
	// ft_printf("processDoubleQuote end \n");
    return buf;
}

char *quotevar(t_shell *shell, const char **s) {
    char *result = strdup(""); // Start with an empty string

	if (!isSpecialOperator(*s))
	{
		while (**s && !isspace((unsigned char)**s) && !(isSpecialOperator(*s)))
		{
			char *temp = NULL;
			int opLength = isSpecialOperator((*s));
			if (**s == '\'') {
				temp = processSingleQuote(s);
			} else if (**s == '\"') {
				temp = processDoubleQuote(s, shell);
			} else if (**s == '$') {
				size_t advancedPosition = 0;
				temp = expandVariables2(shell, *s, &advancedPosition);
				// *s += advancedPosition - 1; // Adjust for loop increment
				*s += advancedPosition; // Adjust for loop increment
			}
			// else if (opLength > 0)
			// {
			// 	ft_printf("quotevar: operator found\n");
			// 	// if (!temp)
			// 	// {
			// 		temp = ft_strndup((*s), opLength);
			// 		result = temp;
			// 		*s += opLength;
			// 	// }
			// 	break ;
			// }
			else {
				// ft_printf("quotevar |%c|\n", **s);
				temp = malloc(2); // Allocate space for char and null terminator
				temp[0] = **s;
				temp[1] = '\0';
				(*s)++;
			}
			(void)opLength;
			if (temp) {
				char *new_result = ft_strjoin(result, temp);
				free(result); // Free the old result
				result = new_result; // Update the result with the new concatenated string
				free(temp); // Free the temporary string
				// ft_printf("quotevar: while: result = |%s|\n", result);
			}
		}
	}
	else
	if (isSpecialOperator(*s))
	{
		result = ft_strndup((*s), isSpecialOperator(*s));
		*s += isSpecialOperator(*s);
	}
	// ft_printf("quotevar: returning result = |%s|\n", result);
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

		// quote and var logic here
		wvarexpanded = quotevar(shell, &s);

		if (index == 0)
			type = TOKEN_COMMAND;
		else
			type = get_token_type(wvarexpanded);
		addToken(shell, wvarexpanded, type);
		index++;
		parse_heredoc(shell);
		s = skip_delimiters(s, ' ');
	}
	return (NULL);
}

// v3 tokenizer end

void addToken(t_shell *shell, const char *value, int type)
{
	// ft_printf("addToken shell_malloc\n");
	TokenNode *newNode = (TokenNode *)shell_malloc(shell, sizeof(TokenNode));
	if (!newNode) return;

	// ft_printf("addToken shell_strdup\n");
	newNode->token.value = shell_strdup(shell, value); // Use shell_strdup
	newNode->token.type = type;
	// ft_printf("addToken new token type is %d\n", newNode->token.type);
	newNode->next = NULL;

	// ft_printf("addToken token_head NULL?\n");
	if (shell->token_head == NULL)
		shell->token_head = newNode;
	else
	{
		// ft_printf("addToken token_head NULL? else\n");
		TokenNode *current = shell->token_head;
		// ft_printf("addToken while (current->next != NULL)\n");
		while (current->next != NULL)
		{
			// ft_printf("addToken while\n");
			current = current->next;
		}
		// ft_printf("addToken current->next = newNode;\n");
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

void    set_commands(t_shell *shell)
{
	TokenNode	*node;
	
	node = shell->token_head;
	if (node){
		node->token.type=TOKEN_COMMAND;

		while (node->next)
			{
				if (node->token.type == TOKEN_PIPE)
				{
					node=node->next;
					if (node->token.type==TOKEN_ARG)
						node->token.type=TOKEN_COMMAND;
				}
				else
					node=node->next;
			}
	}
}

void create_tokens(t_shell *shell, const char *s)
{
	parse_tokens(shell, s);
	set_commands(shell);
}