/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_exp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:56:22 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/19 16:42:05 by vitenner         ###   ########.fr       */
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



// v4 testing
int getEnvVarLength(const char **input, t_env_var *envVars) {
    const char *varStart = *input;
    while (isalnum((unsigned char)**input) || **input == '_') (*input)++;
    int varNameLength = *input - varStart;
    char varName[varNameLength + 1];
    strncpy(varName, varStart, varNameLength);
    varName[varNameLength] = '\0';

    t_env_var *var = findEnvVar(envVars, varName);
    if (var) {
        return strlen(var->value); // Return length of the variable value
    } else {
        return 0; // If the variable is not found, return 0
    }
}

int calculateExpandedLength(t_shell *shell, const char *input, t_env_var *envVars) {
    int length = 0;
    while (*input) {
        if (*input == '$') {
            if (*(input + 1) == ' ') { // Check for space after $
                length += 2; // Account for $ and the space
                input += 2;
            } else if (*(input + 1) == '?') {
                length += intLength(shell->last_exit_status);
                input += 2;
            // } else if (*(input + 1) == '\0' || *(input + 1) == '$') {
            //     length += 1; // Account for $
            //     input += 1;
            }
            else if (*(input + 1) == '$') {
            length += 1; // Account for the first $
            input++;
            while (*input == '$') {
                length += 1; // Correctly account for subsequent $
                input++;
            }
            } else {
                input++; // Move past $ to start variable name
                length += getEnvVarLength(&input, envVars);
            }
        } else {
            length++;
            input++;
        }
    }
    return length + 1; // Account for null terminator
}



// can refactor this by replacing envVars with shell
void replaceEnvVar(const char **input, char **output, t_env_var *envVars) {
    const char *varStart = ++(*input);
    while (isalnum((unsigned char)**input) || **input == '_') (*input)++;
    int varNameLength = *input - varStart;
    char varName[varNameLength + 1];
    strncpy(varName, varStart, varNameLength);
    varName[varNameLength] = '\0';
    t_env_var *var = findEnvVar(envVars, varName);
    if (var) {
        strcpy(*output, var->value); // Replace variable with value
        *output += strlen(var->value);
    }
    // Note: If the variable is not found, nothing is copied. Adjust if needed.
}

char	*cpy_exit_code(char *str, int n)
{
	int		i;

	i = ft_intlen((n));
	str[i] = '\0';
	if (str)
	{
		while (i > 0)
		{
			str[--i] = ft_abs(n % 10) + 48;
			n /= 10;
		}
	}
    // ft_printf("exitcode |%d| |%s|\n", n, str);
	return (str);
}

size_t replaceVariables(t_shell *shell, const char *input, char *output, t_env_var *envVars) {
        const char *startInput = input; // Keep track of the start position

    while (*input)
    {
        if (*input == '$')
        {
            if (*(input + 1) == ' ') 
            { // Check for space after $
                break;
                // *output++ = *input++; // Copy $ and space literally
                // *output++ = *input++;
            }
            else if (*(input + 1) == '?' )
            {
                cpy_exit_code(output, shell->last_exit_status);
                output += intLength(shell->last_exit_status);
                input += 2;
            // } else if (*(input + 1) == '\0' || *(input + 1) == '$') {
            // } else if (*(input + 1) == '\0' || *(input + 1) == '$' || *(input + 1) == '\"') {
                // ft_printf("replaceVariables  } else if (*(input + 1) == '\0' || *(input + 1) == '$') { output |%s|\n", output);
            //     *output++ = *input++;
            //     if (*input == '$') {
            //         *output++ = *input++;
            //     }
            }
            else if (*(input + 1) == '$' || *(input + 1) == '\"')
            {
                // Directly handle consecutive dollar signs here
                *output++ = *input++; // Copy the first $ and move to the next character
                if (*(input) == '$')
                { // Check if the next character is also a $
                    while (*input == '$')
                        *output++ = *input++; // Copy and advance for each subsequent $
                }
            }
            else if (*(input + 1) == '\0')
            {
                *output++ = *input++; // Just copy the $ if it's the end of the string
            } else
            {
                // ft_printf("replaceVariables else input+1 |%c| output |%s|\n", *(input+1), output);
                replaceEnvVar(&input, &output, envVars); // Use the new function
            }
        }
        // added break condition for tokenizer:
        else if (*input == '\"' || *input == '\'' || *input == ' ')
            break ;
        // 
        else {
            *output++ = *input++;
        }
    }
    *output = '\0';
    // ft_printf("replaceVariables output |%s|\n", output);
    return input - startInput; // Return how far we advanced

}



// can refactor this by replacing envVars with shell
char* expandVariables(t_shell *shell, const char *input)
{
    int finalLength = calculateExpandedLength(shell, input, shell->env_head);
    char *expanded = (char *)malloc(finalLength);
    if (!expanded) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    size_t advancedPosition = replaceVariables(shell, input, expanded, shell->env_head);
    // ft_printf("expandVariables returns %s\n", expanded);
    (void)advancedPosition;
    return expanded;
}

char* expandVariables2(t_shell *shell, const char *input, size_t *advancedPosition)
{
    int finalLength = calculateExpandedLength(shell, input, shell->env_head);
    char *expanded = (char *)malloc(finalLength);
    if (!expanded) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    size_t position = replaceVariables(shell, input, expanded, shell->env_head);
    if (advancedPosition != NULL) {
        *advancedPosition = position; // Update the external pointer with the advanced position
    }
    // ft_printf("expandVariables2 returns %s\n", expanded);
    return expanded;
}




// // // v3 OK
// int getEnvVarLength(const char **input, t_env_var *envVars) {
//     const char *varStart = *input;
//     while (isalnum((unsigned char)**input) || **input == '_') (*input)++;
//     int varNameLength = *input - varStart;
//     char varName[varNameLength + 1];
//     strncpy(varName, varStart, varNameLength);
//     varName[varNameLength] = '\0';

//     t_env_var *var = findEnvVar(envVars, varName);
//     if (var) {
//         return strlen(var->value); // Return length of the variable value
//     } else {
//         return 0; // If the variable is not found, return 0
//     }
// }

// // can refactor this by replacing envVars with shell
// int calculateExpandedLength(t_shell *shell, const char *input, t_env_var *envVars)
// {
//     int length = 0;
//     while (*input) {
//         if (*input == '$') {
//             if (*(input + 1) == '?') {
//                 length += intLength(shell->last_exit_status);
//                 input += 2;
//             } else if (*(input + 1) == '\0' || *(input + 1) == '$') {
//                 length += 1; // Account for $
//                 input += 1;
//             } else {
//                 input++; // Move past $ to start variable name
//                 length += getEnvVarLength(&input, envVars);
//                 // No need to advance input here; getEnvVarLength does it
//             }
//         } else {
//             length++;
//             input++;
//         }
//     }
//     return length + 1; // Account for null terminator
// }


// // can refactor this by replacing envVars with shell
// void replaceEnvVar(const char **input, char **output, t_env_var *envVars) {
//     const char *varStart = ++(*input);
//     while (isalnum((unsigned char)**input) || **input == '_') (*input)++;
//     int varNameLength = *input - varStart;
//     char varName[varNameLength + 1];
//     strncpy(varName, varStart, varNameLength);
//     varName[varNameLength] = '\0';
//     t_env_var *var = findEnvVar(envVars, varName);
//     if (var) {
//         strcpy(*output, var->value); // Replace variable with value
//         *output += strlen(var->value);
//     }
//     // Note: If the variable is not found, nothing is copied. Adjust if needed.
// }

// char	*cpy_exit_code(char *str, int n)
// {
// 	int		i;

// 	i = ft_intlen((n));
// 	str[i] = '\0';
// 	if (str)
// 	{
// 		while (i > 0)
// 		{
// 			str[--i] = ft_abs(n % 10) + 48;
// 			n /= 10;
// 		}
// 	}
//     // ft_printf("exitcode |%d| |%s|\n", n, str);
// 	return (str);
// }

// // can refactor this by replacing envVars with shell
// void replaceVariables(t_shell *shell, const char *input, char *output, t_env_var *envVars) {
//     while (*input) {
//         while (*input == '$') {
//             if (*(input + 1) == '?' ) {
//                 // strcpy(output, "hello");
//                 cpy_exit_code(output, shell->last_exit_status);
//                 // output = export_exit_code(shell);
//                 output += intLength(shell->last_exit_status);
//                 (void)shell;
//                 // output += 5;
//                 input += 2;
//             } else if (*(input + 1) == '\0' || *(input + 1) == '$' || isspace(*(input + 1))) {
//             // } else if (*(input + 1) == '\0' || *(input + 1) == '$') {
//                 *output++ = *input++;
//                 if (*input == '$') {
//                     *output++ = *input++;
//                 }
//             } else {
//                 replaceEnvVar(&input, &output, envVars); // Use the new function
//                 // Note: input and output are already advanced in replaceEnvVar
//             }
//         }
//         if (*input) {
//             *output++ = *input++;
//         }
//     }
//     *output = '\0';
// }


// // can refactor this by replacing envVars with shell
// char* expandVariables(t_shell *shell, const char *input, t_env_var *envVars)
// {
//     int finalLength = calculateExpandedLength(shell, input, envVars);
//     char *expanded = (char *)malloc(finalLength);
//     if (!expanded) {
//         printf("Memory allocation failed\n");
//         return NULL;
//     }
    
//     replaceVariables(shell, input, expanded, envVars);
    
//     return expanded;
// }


