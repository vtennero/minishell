/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_exp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:56:22 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/15 16:00:36 by vitenner         ###   ########.fr       */
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

// v3 testing
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

// can refactor this by replacing envVars with shell
int calculateExpandedLength(t_shell *shell, const char *input, t_env_var *envVars)
{
    int length = 0;
    while (*input) {
        if (*input == '$') {
            if (*(input + 1) == '?') {
                length += intLength(shell->last_exit_status);
                input += 2;
            } else if (*(input + 1) == '\0' || *(input + 1) == '$') {
                length += 1; // Account for $
                input += 1;
            } else {
                input++; // Move past $ to start variable name
                length += getEnvVarLength(&input, envVars);
                // No need to advance input here; getEnvVarLength does it
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

// can refactor this by replacing envVars with shell
void replaceVariables(t_shell *shell, const char *input, char *output, t_env_var *envVars) {
    while (*input) {
        while (*input == '$') {
            if (*(input + 1) == '?' ) {
                // strcpy(output, "hello");
                cpy_exit_code(output, shell->last_exit_status);
                // output = export_exit_code(shell);
                output += intLength(shell->last_exit_status);
                (void)shell;
                // output += 5;
                input += 2;
            } else if (*(input + 1) == '\0' || *(input + 1) == '$') {
                *output++ = *input++;
                if (*input == '$') {
                    *output++ = *input++;
                }
            } else {
                replaceEnvVar(&input, &output, envVars); // Use the new function
                // Note: input and output are already advanced in replaceEnvVar
            }
        }
        if (*input) {
            *output++ = *input++;
        }
    }
    *output = '\0';
}



// v2 OK
// void replaceVariables(const char *input, char *output, t_env_var *envVars) {
//     while (*input) {
//         while (*input == '$') { // Use a while loop to continuously check for $
//             if (*(input + 1) == '?' ) {
//                 strcpy(output, "1337"); // Replace $? with hello
//                 output += 4; // Move past "hello"
//                 input += 2; // Move past $? in the input
//             } else if (*(input + 1) == '\0' || *(input + 1) == '$') {
//                 *output++ = *input++; // Copy $ and move to the next character
//                 // Don't skip the next $ if it's part of a sequence like $$
//             } else {
//                 // Process a variable name
//                 const char *varStart = ++input;
//                 while (isalnum((unsigned char)*input) || *input == '_') input++;
//                 int varNameLength = input - varStart;
//                 char varName[varNameLength + 1];
//                 strncpy(varName, varStart, varNameLength);
//                 varName[varNameLength] = '\0';
//                 t_env_var *var = findEnvVar(envVars, varName);
//                 if (var) {
//                     strcpy(output, var->value); // Replace variable with value
//                     output += strlen(var->value);
//                 }
//                 // If no valid variable found, do not append anything.
//             }
//         }
//         if (*input) { // Check if *input is not at the end of the string after processing $
//             *output++ = *input++; // Copy other characters directly
//         }
//     }
//     *output = '\0'; // Null-terminate the output string
// }



// int calculateExpandedLength(const char *input, t_env_var *envVars) {
//     int length = 0;
//     while (*input) {
//         if (*input == '$') {
//             if (*(input + 1) == '?') {
//                 length += 4; // "1337"
//                 input += 2;
//             } else if (*(input + 1) == '\0' || *(input + 1) == '$') {
//                 // Include both $ if the next character is also a $
//                 length += 1; // single $
//                 input++; // move past the first $
//                 if (*(input) == '$') {
//                     length += 1; // add another $ if the next is $
//                 }
//             } else {
//                 const char *varStart = ++input;
//                 while (isalnum((unsigned char)*input) || *input == '_') input++;
//                 int varNameLength = input - varStart;
//                 char varName[varNameLength + 1];
//                 strncpy(varName, varStart, varNameLength);
//                 varName[varNameLength] = '\0';
//                 t_env_var *var = findEnvVar(envVars, varName);
//                 if (var) {
//                     length += strlen(var->value); // Add variable value length
//                 } else {
//                     length += varNameLength; // Assume variable name length if var not found
//                 }
//             }
//         } else {
//             length++;
//             input++;
//         }
//     }
//     return length + 1; // +1 for null terminator
// }


// v1, this works
// void replaceVariables(const char *input, char *output, t_env_var *envVars) {
//     while (*input) {
//         if (*input == '$') {
//             const char *varStart = ++input;
//             while (isalnum((unsigned char)*input) || *input == '_') input++;
//             int varNameLength = input - varStart;
//             char varName[varNameLength + 1];
//             strncpy(varName, varStart, varNameLength);
//             varName[varNameLength] = '\0';
//             t_env_var *var = findEnvVar(envVars, varName);
//             if (var) {
//                 strcpy(output, var->value); // Replace variable with value
//                 output += strlen(var->value);
//             }
//         } else {
//             *output++ = *input++;
//         }
//     }
//     *output = '\0'; // Null-terminate the output string
// }

// v1, this works

// int calculateExpandedLength(const char *input, t_env_var *envVars) {
//     int length = 0;
//     while (*input) {
//         if (*input == '$') {
//             const char *varStart = ++input;
//             while (isalnum((unsigned char)*input) || *input == '_') input++;
//             int varNameLength = input - varStart;
//             char varName[varNameLength + 1];
//             strncpy(varName, varStart, varNameLength);
//             varName[varNameLength] = '\0';
//             t_env_var *var = findEnvVar(envVars, varName);
//             if (var) {
//                 length += strlen(var->value); // Add variable value length
//             } else {
//                 length += varNameLength; // Add length of variable name if not found
//             }
//         } else {
//             length++;
//             input++;
//         }
//     }
//     return length + 1; // +1 for null terminator
// }

// can refactor this by replacing envVars with shell
char* expandVariables(t_shell *shell, const char *input, t_env_var *envVars)
{
    int finalLength = calculateExpandedLength(shell, input, envVars);
    char *expanded = (char *)malloc(finalLength);
    if (!expanded) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    replaceVariables(shell, input, expanded, envVars);
    
    return expanded;
}
