/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_exp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:56:22 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/31 22:37:53 by vitenner         ###   ########.fr       */
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
            if (*(input + 1) == ' ') {
                length += 2;
                input += 2;
            } else if (*(input + 1) == '?') {
                length += process_double_quotegth(shell->last_exit_status);
                input += 2;
            }
            else if (*(input + 1) == '$') {
            length += 1;
            input++;
            while (*input == '$') {
                length += 1;
                input++;
            }
            } else {
                input++;
                length += getEnvVarLength(&input, envVars);
            }
        } else {
            length++;
            input++;
        }
    }
    return length + 1;
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

	i = process_double_quote((n));
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
            // ft_printf("replaceVariables is $ |%s|\n", input);
            if (*(input + 1) == ' ')
            { // Check for space after $
                break;
                // *output++ = *input++; // Copy $ and space literally
                // *output++ = *input++;
            }
            else if (*(input + 1) == '?' )
            {
            // ft_printf("replaceVariables is ? input=|%s| output=|%s|\n", input, output);
                cpy_exit_code(output, shell->last_exit_status);
            // ft_printf("replaceVariables is ? input=|%s| output=|%s|\n", input, output);
                output += process_double_quotegth(shell->last_exit_status);
                input += 2;
            // ft_printf("replaceVariables is ? input=|%s| output=|%s|\n", input, output);
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
    return input - startInput;

}

char* expandVariables(t_shell *shell, const char *input)
{
    int finalLength = calculateExpandedLength(shell, input, shell->env_head);
    char *expanded = (char *)malloc(finalLength);
    if (!expanded) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    size_t advancedPosition = replaceVariables(shell, input, expanded, shell->env_head);
    (void)advancedPosition;
    return expanded;
}

char* expandVariables2(t_shell *shell, const char *input, size_t *advancedPosition)
{
    int finalLength = calculateExpandedLength(shell, input, shell->env_head);
    char *expanded = shell_malloc(shell,finalLength*sizeof(char*));

    if (!expanded) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    size_t position = replaceVariables(shell, input, expanded, shell->env_head);
    if (advancedPosition != NULL) {
        *advancedPosition = position;
    }
    return expanded;
}
