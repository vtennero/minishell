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

// v3 tokenizer

int isspace_not_eol(int ch)
{
    return (ch == ' ' || ch == '\f' || ch == '\r' || ch == '\t' || ch == '\v');
}

// Mock implementation of processVar
char *processVar() {
    // This function would process the variable and return its value
    // For now, just return a duplicate of var for illustration
	// (void)var;
    return strdup("[VAR_PLACEHOLDER]"); // Skip the '$' character
    // return strdup(var + 1); // Skip the '$' character
}



char *processSingleQuote(const char **s)
{
	ft_printf("processSingleQuote start s |%s|\n", *s);
	(*s)++;
    const char *start = *s; // Skip the initial quote
    while (**s && **s != '\'')
		(*s)++;
	ft_printf("processSingleQuote s after while |%s|\n", *s);
	int len = *s - start;
	ft_printf("processSingleQuote len |%d|\n", len);
	char *result = strndup(start, len);
	ft_printf("processSingleQuote s before going past word |%s|\n", *s);
    // if (**s == '\'') (*s)++;
	// (*s) += len + 1;
	ft_printf("processSingleQuote s after going past word |%s|\n", *s);
	ft_printf("processSingleQuote result |%s|\n", result);
    return result;
}


int	get_non_expanded_var_length(char *var)
{
    int count = 1; // Counter for valid characters
    int index = 0; // Current index in the string
    int foundLetter = 0; // Flag to track if at least one letter has been found

	ft_printf("get_non_expanded_var_length %s\n", var);
	var++;
	ft_printf("get_non_expanded_var_length %s\n", var);
    while(var[index] != '\0') {
        if(!isalpha(var[index]) && !isdigit(var[index])) {
            // If the character is not a letter or digit, break the loop
            break;
        }
        if(isalpha(var[index])) {
            foundLetter = 1; // Set the flag if a letter is found
        }
        count++; // Increment the count of valid characters
        index++; // Move to the next character
    }
	ft_printf("get_non_expanded_var_length %d\n", count);
	if (!foundLetter)
		count = 0;
	return (count);
}

// char *processDoubleQuote(const char **s, t_shell *shell)
// {
// 	char	*result;
// 	char	*result2;

// 	ft_printf("processDoubleQuote start s |%s|\n", *s);
// 	(*s)++;
//     const char *start = *s; // Skip the initial quote
// 	result2 = NULL;
//     while (**s && **s != '\"')
// 	{
//         if (**s == '$')
// 		{
// 			result2 = expandVariables(shell, *s);
// 			ft_printf("processDoubleQuote result2 %s\n", result2);
// 			(*s)++;
// 			while (**s && isalnum(**s))
// 				(*s)++;
// 		}
// 		else
// 			(*s)++;
// 	}
// 	ft_printf("processDoubleQuote s after while |%s|\n", *s);
// 	int len = *s - start;
// 	ft_printf("processDoubleQuote len |%d|\n", len);
// 	result = strndup(start, len);
// 	ft_printf("processDoubleQuote result |%s|\n", result);
// 	result = ft_strjoin(result2, result);
// 	// ft_printf("processDoubleQuote s before going past word |%s|\n", *s);
// 	// ft_printf("processDoubleQuote s after going past word |%s|\n", *s);
// 	ft_printf("processDoubleQuote result |%s|\n", result);
//     return result;
// }

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
		ft_printf("processDoubleQuote while (*s < endQuote)  |%s|\n", *s);
        if (**s == '$') {
            // a. Expand the variable
            char *expanded = expandVariables(shell, *s);
			ft_printf("processDoubleQuote char *expanded = expandVariables(shell, *s); |%s|\n", expanded);
			if (!buf)
				newBuf = expanded;
			else
            	newBuf = ft_strjoin(buf, expanded);
            // free(buf); // Free the old buffer
            // free(expanded); // Free the expanded string
            buf = newBuf;
			ft_printf("processDoubleQuote buf |%s|\n", buf);
			textLen = get_non_expanded_var_length((char * )(*s));
			ft_printf("processDoubleQuote s |%s|\n", *s);
            *s += textLen;
			ft_printf("processDoubleQuote s |%s|\n", *s);
            // Move pointer s by n characters, where n is the length of the variable
            // Assuming expandVariables moves *s to the end of the variable
        } else {
			ft_printf("processDoubleQuote while (*s < endQuote) else not $ |%s|\n", *s);
            // b. Duplicate text until the next $ or the end quote
            const char *nextDollar = strchr(*s, '$');
            if (!nextDollar || nextDollar > endQuote) {
                nextDollar = endQuote; // If no $ is found, copy until the end quote
            }
            textLen = nextDollar - *s;
            char *duplicatedText = strndup(*s, textLen);
			ft_printf("processDoubleQuote duplicatedText |%s|\n", duplicatedText);
			if (!buf)
				newBuf = duplicatedText;
			else
            	newBuf = ft_strjoin(buf, duplicatedText);
            // free(buf); // Free the old buffer
            // free(duplicatedText); // Free the duplicated text
            buf = newBuf;
			ft_printf("processDoubleQuote else buf |%s|\n", buf);

            // Move the pointer to the position of the next $ or end quote
            *s += textLen;
        }
    }

    // Move past the closing quote for the next iteration
    if (**s == '\"') (*s)++;
    
    return buf;
}

// char *processDoubleQuote(const char **s, t_shell *shell) {
//     const char *start = *s + 1;
//     char *result = malloc(strlen(start) + 1); // Allocate max needed size
//     char *temp = result;
// 	char	*varend;
// 	(void)varend;
// 	(void)shell;
//     while (**s && **s != '\"')
// 	{
//         if (**s == '$')
// 		{
// 				(*s)++; // Move past the '$'
// 				const char *varend = strchr(*s, ' ') ? strchr(*s, ' ') : strchr(*s, '\0');
// 				char *varName = strndup(*s, varend - *s); // Extract variable name

// 				// Assuming processVar takes the variable name and returns its value
// 				char *varValue = processVar(varName); // Get the placeholder or actual value

// 				// Ensure there's enough space in temp to append varValue
// 				// This example directly appends, but you might need to realloc if temp isn't large enough
// 				// strcat(temp, varValue);
// 				temp = ft_strjoin(temp, varValue);

// 				// Move temp forward by the length of varValue
// 				temp += strlen(varValue);

// 				// Free the dynamically allocated strings if necessary
// 				free(varName);
// 				free(varValue); // Only free if processVar returns a dynamically allocated string

// 				// Advance *s to the end of the variable name
// 				*s = varend;
//         } else 
//             *temp++ = **s;
//         (*s)++;
//     }
//     *temp = '\0'; // Null-terminate the string
//     if (**s == '\"') (*s)++;
//     char *finalResult = strdup(result); // Duplicate the correctly sized string
//     free(result);
//     return finalResult;
// }

char *quotevar(t_shell *shell, const char **s)
{
    char *buffer = malloc(1024); // Initial buffer, adjust size as needed
    char *bufPtr = buffer;
    while (**s && !isspace((unsigned char)**s)) {
        if (**s == '\'') {
			ft_printf("quotevar: s before processSingle quotes |%s|\n", *s);
            char *temp = processSingleQuote(s);
            strcpy(bufPtr, temp);
			ft_printf("quotevar: s after processSingle quotes |%s|\n", *s);
			ft_printf("quotevar: if (**s == ') bufPtr  |%s|\n", bufPtr);
            bufPtr += strlen(temp);
            free(temp);
        } else if (**s == '\"') {
            char *temp = processDoubleQuote(s, shell);
            strcpy(bufPtr, temp);
            bufPtr += strlen(temp);
            free(temp);
		}
		else if (**s == '$')
		{
			// expand variables
        } else {
            *bufPtr++ = **s;
            (*s)++;
        }
    }
    *bufPtr = '\0';
    char *finalResult = strdup(buffer);
    free(buffer);
	ft_printf("quotevar: final result |%s|\n", finalResult);
    return finalResult;
}



// int find_closing_quote(const char *str, char c)
// {
// 	char	*ptr;
//     int i = 1;
//     // Find the initial occurrence of the c
//     while (ptr[i] != '\0' && ptr[i] != c)
//         i++;
//     // Return the index of the next occurrence of the c
//     if (ptr[i] == '\0')
//         return 0;
//     else
//         return i;
// }

// char	*get_next_chunk(char *buf, char *newchunk)
// {
// 	char	*newbuf;

// 	if (!buf)
// 		newbuf = ft_strdup(newchunk);
// 	else
// 		newbuf = ft_strlcat(newbuf, buf, ft_strlen(newchunk) + 1);
// 	return (newbuf);
// }

// char	*quotevar(t_shell* shell, char **s)
// {
// 	char	*buf;
// 	char	*transformed;

// 	while (*s && !isspace_noteol(*s))
// 	{
// 		if (*s == '\'')
// 		{
// 			transformed = processSingleQuotes(*s);
// 			buf = get_next_chunk(buf, transformed);
// 		}
// 		if (*s == '\"')
// 		{
// 			transformed = processDoubleQuotes(*s);
// 			buf = get_next_chunk(buf, transformed);
// 		}
// 		// if (*s == '$')
// 		// {
// 		// 	// transformed = expandVariables()
// 		// 	buf = get_next_chunk(buf, transformed);

// 		// }
// 		(*s)++;
// 	}

// 	return (buf);
// }

// char *get_next_chunk(char *buf, const char *newchunk, size_t len)
// {
//     if (!buf)
//     {
//         buf = malloc(len + 1); // Allocate memory for new chunk
//         if (!buf) return NULL; // Check for allocation failure
//         strncpy(buf, newchunk, len); // Copy the chunk into buf
//         buf[len] = '\0'; // Null-terminate the string
//     }
//     else
//     {
//         size_t buflen = strlen(buf);
//         char *newbuf = realloc(buf, buflen + len + 1); // Resize buf to fit new chunk
//         if (!newbuf) return NULL; // Check for allocation failure
//         strncpy(newbuf + buflen, newchunk, len); // Copy the chunk into the new part of buf
//         newbuf[buflen + len] = '\0'; // Null-terminate the string
//         buf = newbuf;
//     }
//     return buf;
// }

// char *quotevar(t_shell *shell, const char **s)
// {
// 	(void)shell;
//     char *buf = NULL;
//     while (*s && **s && !(isspace_not_eol(**s)))
//     {
//         // if (isspace_not_eol(**s))
//         // {
//         //     // Skip spaces
//         //     (*s)++;
//         // }
//         // else
//         // {
//             // Find the nearest quote or the end of the string
//             char *next_single_quote = strchr(*s, '\'');
//             char *next_double_quote = strchr(*s, '\"');
//             char *next_quote = NULL;

//             if (!next_single_quote) next_quote = next_double_quote;
//             else if (!next_double_quote) next_quote = next_single_quote;
//             else next_quote = (next_single_quote < next_double_quote) ? next_single_quote : next_double_quote;

//             if (next_quote)
//             {
//                 // Copy everything up to the next quote
//                 size_t len = next_quote - *s;
//                 buf = get_next_chunk(buf, *s, len);
// 				ft_printf("quotevar: nextquote buf |%s|\n", buf);
//                 *s += len; // Move the pointer forward
//             }
//             else
//             {
//                 // No more quotes found, copy the rest of the string
//                 size_t len = strlen(*s);
//                 buf = get_next_chunk(buf, *s, len);
// 				ft_printf("quotevar: else buf |%s|\n", buf);
//                 *s += len; // Move the pointer to the end
//                 break; // No more processing needed
//             // }
//         }
//     }
// 	ft_printf("quotevar: buf |%s|\n", buf);
//     return buf;
// }


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
	if (ft_strcmp(token_text, "<") == 0) {
		return TOKEN_REDIR_IN;
	} else if (ft_strcmp(token_text, ">") == 0) {
		return TOKEN_REDIR_OUT;
	} else if (ft_strcmp(token_text, ">>") == 0) {
		return TOKEN_REDIR_APPEND;
	} else if (ft_strcmp(token_text, "<<") == 0) {
		return TOKEN_REDIR_HEREDOC;
	} else if (ft_strcmp(token_text, "|") == 0) {
		return TOKEN_PIPE;
	} else if (ft_strcmp(token_text, "$?") == 0)
		return TOKEN_EXIT_STATUS;
	return TOKEN_ARG; // Default case, can be TOKEN_COMMAND or TOKEN_ARG based on context
}





// char    *parse_tokens(t_shell *shell, const char *s)
// {
// 	char    *processedQuotes;
// 	char    *wvarexpanded;
// 	int     type;
// 	int     isInSingleQuotes = 0;
// 	int		isInDoubleQuotes = 0; // Track quote states
// 	int		index;
// 	// QuoteType		quoteType;

// 	index = 0;
// 	// quoteType = NO_QUOTES;
// 	while (*s)
// 	{
// 		s = skip_delimiters(s, ' ');
// 		if (!*s)
// 			break;
// 		const char *start = s; // Start of the word
// 		// while (*s && !(isspace(*s)) || isInSingleQuotes || isInDoubleQuotes)
// 		while (*s && (!isspace(*s) || isInSingleQuotes || isInDoubleQuotes))
// 		{
// 			if (*s == '\'' && !isInDoubleQuotes) {
// 				toggleQuoteState(&isInSingleQuotes);
// 			} else if (*s == '\"' && !isInSingleQuotes) {
// 				toggleQuoteState(&isInDoubleQuotes);
// 			}
// 			s++;
// 		}

// 		int wordLength = s - start;
// 		char *word = strndup(start, wordLength); // Copy the current word
// 		// ft_printf("word |%s|\n", word);
// 		processedQuotes = reviewquotes(word);
// 		// processedQuotes = reviewquotes(word, &quoteType);
// 		// ft_printf("quoteType |%d|\n", quoteType);
// 		// if (quoteType == SINGLE_QUOTES)
// 		// wvarexpanded = processedQuotes;
// 		// if (shouldExpandVariable(processedQuotes))
// 		// else
// 			wvarexpanded = expandVariables(shell, processedQuotes, shell->env_head);
// 		ft_printf("with quotes expanded |%s|\n", processedQuotes);
// 		ft_printf("with variables expanded  |%s|\n", wvarexpanded);
// 		if (index == 0)
// 			type = 0;
// 		else
// 			type = get_token_type(wvarexpanded);
// 		addToken(shell, wvarexpanded, type);
// 		index++;
// 		parse_heredoc(shell);
// 		s = skip_delimiters(s, ' ');
// 		// while (*s == ' ')
// 			// s++;
// 	}
// 	return (NULL);
// }

void    set_commands(t_shell *shell)
{
	TokenNode *node = shell->token_head;
	if (node){
		node->token.type=TOKEN_COMMAND;

		while (node->next)
			{
				if (node->token.type == TOKEN_PIPE)
				{
					node=node->next;
					node->token.type=TOKEN_COMMAND;
				}
				else
					node=node->next;
			}

	}

}

void    createtokensv2(t_shell *shell, const char *s)
{
	// ft_printf("createtokensv2 START\n");
	// printf("str ois %s \n",s);
	parse_tokens(shell, s);
	// printTokens(shell->token_head);
	set_commands(shell);
 
	// ft_printf("createtokensv2 END\n");

}

void create_tokens(t_shell *shell, const char *s)
{
	createtokensv2(shell, s);
}