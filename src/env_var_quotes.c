/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:50:47 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/15 13:01:54 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_var *findEnvVar(t_env_var *head, const char *key) {
    while (head != NULL) {
        if (strcmp(head->key, key) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void replaceVariables(const char *input, char *output, t_env_var *envVars) {
    while (*input) {
        if (*input == '$') {
            const char *varStart = ++input;
            while (isalnum((unsigned char)*input) || *input == '_') input++;
            int varNameLength = input - varStart;
            char varName[varNameLength + 1];
            strncpy(varName, varStart, varNameLength);
            varName[varNameLength] = '\0';
            t_env_var *var = findEnvVar(envVars, varName);
            if (var) {
                strcpy(output, var->value); // Replace variable with value
                output += strlen(var->value);
            }
        } else {
            *output++ = *input++;
        }
    }
    *output = '\0'; // Null-terminate the output string
}
int calculateExpandedLength(const char *input, t_env_var *envVars) {
    int length = 0;
    while (*input) {
        if (*input == '$') {
            const char *varStart = ++input;
            while (isalnum((unsigned char)*input) || *input == '_') input++;
            int varNameLength = input - varStart;
            char varName[varNameLength + 1];
            strncpy(varName, varStart, varNameLength);
            varName[varNameLength] = '\0';
            t_env_var *var = findEnvVar(envVars, varName);
            if (var) {
                length += strlen(var->value); // Add variable value length
            } else {
                length += varNameLength; // Add length of variable name if not found
            }
        } else {
            length++;
            input++;
        }
    }
    return length + 1; // +1 for null terminator
}

char* expandVariables(const char *input, t_env_var *envVars)
{
    int finalLength = calculateExpandedLength(input, envVars);
    char *expanded = (char *)malloc(finalLength);
    if (!expanded) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    replaceVariables(input, expanded, envVars);
    
    return expanded;
}

void toggleQuoteState(int *quoteState)
{
    *quoteState = !(*quoteState);
}

void processCharacter(char c, char **output, int *isInSingleQuotes, int *isInDoubleQuotes) {
    if (c == '\'' && !(*isInDoubleQuotes)) {
        toggleQuoteState(isInSingleQuotes);
    } else if (c == '\"' && !(*isInSingleQuotes)) {
        toggleQuoteState(isInDoubleQuotes);
    } else {
        *(*output) = c;
        (*output)++;
    }
}

void transformQuotes(const char *input, char *output) {
    int isInSingleQuotes = 0, isInDoubleQuotes = 0;
    char *outputPtr = output;

    while (*input) {
        processCharacter(*input, &outputPtr, &isInSingleQuotes, &isInDoubleQuotes);
        input++;
    }
    *outputPtr = '\0'; // Null-terminate the output string
}

char 	*reviewquotes(char *input)
{
    char *output = (char *)malloc(strlen(input) + 1); // Allocate memory for the output string

    if (output == NULL) {
        printf("Memory allocation failed\n");
        return (NULL);
    }

    transformQuotes(input, output);
    printf("reviewquotes Transformed string: %s\n", output);
	return (output);
}



char    *quotevarhandlerv2(t_shell *shell, const char *s)
{
    char    *processedQuotes;
    char    *wvarexpanded;
    int     type;
    int isInSingleQuotes = 0, isInDoubleQuotes = 0; // Track quote states

    while (*s)
    {
		s = skip_delimiters(s, ' ');
		if (!*s)
            break;
        const char *start = s; // Start of the word
        // while (*s && !(isspace(*s)) || isInSingleQuotes || isInDoubleQuotes)
        while (*s && (!isspace(*s) || isInSingleQuotes || isInDoubleQuotes))
        {
            if (*s == '\'' && !isInDoubleQuotes) {
                toggleQuoteState(&isInSingleQuotes);
            } else if (*s == '\"' && !isInSingleQuotes) {
                toggleQuoteState(&isInDoubleQuotes);
            }
            s++;
        }

        int wordLength = s - start;
        char *word = strndup(start, wordLength); // Copy the current word
        processedQuotes = reviewquotes(strdup(word));
        wvarexpanded = expandVariables(processedQuotes, shell->env_head);
        // ft_printf("with quotes expanded |%s|\n", processedQuotes);
        // ft_printf("with variables expanded  |%s|\n", wvarexpanded);
        type = get_token_type(wvarexpanded);
        addToken(shell, wvarexpanded, type);

        while (*s == ' ')
            s++;
	}
    ft_printf("quotevarhandlerv2 END\n");
    return (NULL);
}

void    set_commands(t_shell *shell)
{
    TokenNode *node = shell->token_head;
    node->token.type=TOKEN_COMMAND;
    while (node->next)
    {
        if (node->token.type == TOKEN_PIPE || node->token.type == TOKEN_EOL)
        {
            node=node->next;
            node->token.type=TOKEN_COMMAND;
        }
        else
            node=node->next;
    }
}



void    createtokensv2(t_shell *shell, const char *s)
{
    // ft_printf("createtokensv2 START\n");
    quotevarhandlerv2(shell, s);
    set_commands(shell);
    // ft_printf("createtokensv2 END\n");

}



