/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:50:47 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/15 12:45:33 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// v3

// // to check if variable exists
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
    // First, calculate the required length for the expanded string
    int finalLength = calculateExpandedLength(input, envVars);
    char *expanded = (char *)malloc(finalLength);
    if (!expanded) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    // Then, replace variables with their values
    replaceVariables(input, expanded, envVars);
    
    return expanded;
}


// // to calculate variable name length

// int isVariableChar(char c) {
//     return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
// }

// void processVariable(const char **input, char **output, t_env_var *envVars) {
//     (*input)++; // Skip the '$'
//     const char *start = *input;
//     int length = 0;

//     // Calculate variable name length
//     while (isVariableChar(**input)) {
//         (*input)++;
//         length++;
//     }

//     // Copy variable name
//     char *varName = malloc(length + 1);
//     if (varName == NULL) {
//         fprintf(stderr, "Memory allocation failed for variable name\n");
//         return;
//     }
//     strncpy(varName, start, length);
//     varName[length] = '\0';

//     // Find and replace variable
//     t_env_var *var = findEnvVar(envVars, varName);
//     if (var) {
//         strcpy(*output, var->value);
//         *output += strlen(var->value);
//     }

//     free(varName);

//     (*input)--; // Adjust because main loop will increment
// }

// // initial functions

// // Toggle the state of being inside or outside quotes
// void toggleQuoteState(int *quoteState) {
//     *quoteState = !(*quoteState);
// }

// // Process each character based on the current quote state
// void processCharacter(char c, char **output, int *isInSingleQuotes, int *isInDoubleQuotes) {
//     if (c == '\'' && !(*isInDoubleQuotes)) {
//         toggleQuoteState(isInSingleQuotes);
//     } else if (c == '\"' && !(*isInSingleQuotes)) {
//         toggleQuoteState(isInDoubleQuotes);
// 	} else if (c == '$' && !(*isInSingleQuotes)) {
//         // When a $ is found outside single quotes, indicate a variable is found
//         ft_printf("VAR found\n");
//     } else {
//         // Copy the character if not in the process of toggling quotes
//         *(*output) = c;
//         (*output)++;
//     }
// }

// // Main transformation function
// void transformQuotes(const char *input, char *output) {
//     int isInSingleQuotes = 0, isInDoubleQuotes = 0;
//     char *outputPtr = output;

//     while (*input) {
//         processCharacter(*input, &outputPtr, &isInSingleQuotes, &isInDoubleQuotes);
//         input++;
//     }
//     *outputPtr = '\0'; // Null-terminate the output string
// }

// int reviewquotes(char *input)
// {
// 	// malloc here
//     char *output = (char *)malloc(strlen(input) + 1); // Allocate memory for the output string

//     if (output == NULL) {
//         printf("Memory allocation failed\n");
//         return 1;
//     }

//     transformQuotes(input, output);
//     printf("Transformed string: %s\n", output);

//     free(output); // Free the allocated memory
//     return 0;
// }


// // // Appends a variable's value to the output
// // void replaceVarWithValue(char **output, t_env_var *var) {
// //     if (var == NULL) return;
// //     strcpy(*output, var->value);
// //     *output += strlen(var->value);
// // }

// // // Append a single character to the output string
// // void appendChar(char **output, char c) {
// //     *(*output) = c;
// //     (*output)++;
// // }

// // // Toggle the state of being inside or outside quotes
// // void toggleQuoteState(int *quoteState) {
// //     *quoteState = !(*quoteState);
// // }

// // // Handle variable detection and replacement
// // void handleVariableCharacter(char c, char **output, t_env_var *envVars, char *varBuffer, int *varBufferIndex) {
// //     if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
// //         // Collecting variable name
// //         varBuffer[(*varBufferIndex)++] = c;
// //     } else {
// //         // End of variable name, try replacing
// //         varBuffer[*varBufferIndex] = '\0'; // Null-terminate var name
// //         t_env_var *var = findEnvVar(envVars, varBuffer);
// //         replaceVarWithValue(output, var);
// //         *varBufferIndex = -1; // Stop collecting, reset buffer index
// //         if (c != '$') { // If current char is not starting new var, append it
// //             appendChar(output, c);
// //         } else {
// //             // Handle consecutive variables
// //             *varBufferIndex = 0;
// //         }
// //     }
// // }



// v2 works great for single and double quotes

// Toggle the state of being inside or outside quotes
void toggleQuoteState(int *quoteState)
{
    *quoteState = !(*quoteState);
}

// Process each character based on the current quote state
void processCharacter(char c, char **output, int *isInSingleQuotes, int *isInDoubleQuotes) {
    if (c == '\'' && !(*isInDoubleQuotes)) {
        toggleQuoteState(isInSingleQuotes);
    } else if (c == '\"' && !(*isInSingleQuotes)) {
        toggleQuoteState(isInDoubleQuotes);
	// } else if (c == '$' && !(*isInSingleQuotes)) {
        // When a $ is found outside single quotes, indicate a variable is found
        // ft_printf("VAR found\n");
    } else {
        // Copy the character if not in the process of toggling quotes
        *(*output) = c;
        (*output)++;
    }
}

// Main transformation function
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
	// malloc here
    char *output = (char *)malloc(strlen(input) + 1); // Allocate memory for the output string

    if (output == NULL) {
        printf("Memory allocation failed\n");
        return (NULL);
    }

    transformQuotes(input, output);
    printf("reviewquotes Transformed string: %s\n", output);
	return (output);
    // free(output); // Free the allocated memory
    // return 0;
}


// for tokens

TokenType get_token_type2(const char* token_text)
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
        // ft_printf("get_token_type TOKEN_EXIT_STATUS\n");
        return TOKEN_EXIT_STATUS;
	return TOKEN_ARG; // Default case, can be TOKEN_COMMAND or TOKEN_ARG based on context
}

char    *quotevarhandlerv2(t_shell *shell, const char *s)
{
    char    *processedQuotes;
    char    *wvarexpanded;
    int     type;
    int isInSingleQuotes = 0, isInDoubleQuotes = 0; // Track quote states

    ft_printf("quotevarhandlerv2 START\n");
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
        ft_printf("with quotes expanded |%s|\n", processedQuotes);
        ft_printf("with variables expanded  |%s|\n", wvarexpanded);
        type = get_token_type2(wvarexpanded);
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
    // if (node && !node->next)
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
    ft_printf("createtokensv2 START\n");
    quotevarhandlerv2(shell, s);
    set_commands(shell);
    ft_printf("createtokensv2 END\n");

}

// v1 works ok for double quotes only:

// void transformQuotes(const char *input, char *output) {
//     int isInQuotes = 0; // Flag to track whether we are inside quotes
//     const char *p = input;
//     while (*p) {
//         if (*p == '\"') { // Detect double quote
//             isInQuotes = !isInQuotes; // Toggle the flag
//         } else {
//             *output++ = *p; // Copy character to output
//         }
//         p++;
//     }
//     *output = '\0'; // Null-terminate the output string
// }

// int reviewdquotes(char *input) {
//     // const char *input = "\"\"hel\"a\"lo\"\"";
//     char *output = malloc(strlen(input) + 1); // Allocate enough memory for the output string

//     if (output == NULL) {
//         printf("Memory allocation failed\n");
//         return 1;
//     }

//     transformQuotes(input, output);
//     printf("Transformed string: %s\n", output);

//     free(output); // Free the allocated memory
//     return 0;
// }



