/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:43:47 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/20 11:24:45 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int handleEnvVar(char **p) {
    char *start = *p + 1;
    char *end = start;
    while (*end && !isspace(*end) && *end != '$') end++; // Find end of env var name
    char varName[end - start + 1]; // +1 for null terminator
    strncpy(varName, start, end - start);
    varName[end - start] = '\0';
    
    char *envValue = getenv(varName); // Get env var value
    int length = 0;
    if (envValue) {
        length = strlen(envValue); // Add length of env var value
    } else {
        length = (end - start + 1); // If var not found, keep original
    }
    *p = end; // Move past the env var name
    return length;
}

int handleSpecialSymbols(char **p) {
    if (**p == '$' && *(*p + 1) == '$')
	{
		ft_printf("Found a $$\n");
		// Handle $$ case here if needed
    }
    // Assuming each special symbol adds 2 to length for demonstration
    *p += 2; // Move past the symbol
    return 2; // Adjust based on actual logic for handling special symbols
}

// this version skips an unexisting variable
int fillNewStringWithEnvVar(char **p, char *dest, t_shell *shell) {
    char *start = *p + 1; // Skip past the $
    char *end = start;
    // Find the end of the variable name
    while (*end && !isspace(*end) && *end != '$') end++;
    char varName[end - start + 1]; // Variable name buffer
    strncpy(varName, start, end - start);
    varName[end - start] = '\0'; // Null-terminate the variable name

    char *envValue = getenv(varName); // Attempt to get the variable's value
    if (envValue) {
        // If the environment variable exists, copy its value to the destination
        strcpy(dest, envValue);
        *p = end; // Move past the variable name in the source string
        return strlen(envValue); // Return the length of the copied value
    } else {
        // If the variable doesn't exist, don't copy anything to dest, effectively removing it
        *p = end; // Still need to move past the variable name in the source string
        return 0; // Return 0 because we didn't add anything to dest

		// alternative: copy unexisting variable name
		//         strncpy(dest, *p, end - *p);
		//         *p = end;
		//         return end - start + 1;
    }
	(void)shell;
}

int fillNewStringWithSpecialSymbols(char **p, char *dest) {
    // This function will handle special symbols ($$, $?, and $ followed by space)
    // The logic here will depend on how you want to treat these symbols
    // For demonstration, let's just copy them as is and advance
    *dest++ = **p; // Copy $
    *dest++ = *(*p + 1); // Copy the next character ($, ?, or space)
    *p += 2; // Advance past the symbols
    return 2; // Return the number of characters added to dest
}

void populateNewString(t_shell *shell, TokenNode *node, char *newStr) {
    char *value = node->token.value;
    char *p = value; // Pointer to iterate through the original string
    char *dest = newStr; // Destination pointer for the new string

    while (*p) {
        if (*p == '$') {
            if (*(p+1) == '$' || *(p+1) == '?' || isspace(*(p+1))) {
                dest += fillNewStringWithSpecialSymbols(&p, dest);
            } else {
                dest += fillNewStringWithEnvVar(&p, dest, shell);
            }
        } else {
            *dest++ = *p++; // Copy regular character and advance
        }
    }
	ft_printf("transformed string: |%s|\n", newStr);
}


void processDQToken(t_shell *shell, TokenNode *node) {
    char *value = node->token.value;
    int newLength = 0;
    char *p = value;

    while (*p) {
        if (*p == '$') {
            if (*(p+1) == '$' || *(p+1) == '?' || isspace(*(p+1))) {
                newLength += handleSpecialSymbols(&p);
            } else {
                newLength += handleEnvVar(&p);
            }
        } else {
            newLength++; // Regular character, just add to length
            p++; // Move to next character
        }
    }

    printf("Calculated new length: %d\n", newLength);
	// (void)shell;
	// After calculating new length
	char *newStr = (char *)shell_malloc(shell, newLength + 1); // +1 for the null terminator
	if (!newStr) {
		perror("Memory allocation failed");
		return; // Exit the function or handle the error as appropriate
	}
	newStr[newLength] = '\0'; // Ensure the string is null-terminated
	populateNewString(shell, node, newStr);
}





void refine_tokens(t_shell *shell)
{
    TokenNode *current = shell->token_head;
    // TokenNode *prev = NULL;
    while (current != NULL)
	{
        if (current->token.type == TOKEN_D_Q)
		{
            processDQToken(shell, current);
        }
        // prev = current;
        current = current->next;
    }
}

