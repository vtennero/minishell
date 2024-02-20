/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:43:47 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/20 15:28:42 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int intLength(int num)
{
	int	length;
    if (num == 0)
        return 1;
    
    length = 0;
    while (num != 0)
	{
        length++;
        num /= 10;
    }

    return (length);
}

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

int handleSpecialSymbols(t_shell *shell, char **p)
{
	int	len;

	len = 0;
    if (**p == '$' && *(*p + 1) == '$')
	{
		// ft_printf("Found a $$\n");
		// Handle $$ case here if needed
    	*p += 2; // Move past the symbol
		len = 2;
    }
	else if ((**p == '$' && *(*p + 1) == '?'))
	{
		len = intLength(shell->last_exit_status);
    	*p += 2; // Move past the symbol
	}
    return (len); // Adjust based on actual logic for handling special symbols
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

int fillNewStringWithSpecialSymbols(t_shell *shell, char **p, char *dest)
{
	int		len;
	char	*exitcodestr;

	len = 0;
    if (**p == '$' && *(*p + 1) == '$')
	{
		*dest++ = **p; // Copy $
		*dest++ = *(*p + 1); // Copy the next character ($, ?, or space)
		*p += 2; // Advance past the symbols
		len = 2;
	}
	else if (**p == '$' && *(*p + 1) == '?')
	{
		exitcodestr = ft_itoa(shell->last_exit_status);
		len = ft_strlen(exitcodestr);
		ft_strncpy(*p, exitcodestr, len);
		// ft_printf("exitcodestr |%s| len %d p %s\n", exitcodestr, len, *p);
		*p += 2; // Advance past the symbols
		free(exitcodestr);
		exitcodestr = NULL;
	}
	else
	{
		*dest++ = **p; // Copy $
		*dest++ = *(*p + 1); // Copy the next character ($, ?, or space)
		*p += 2; // Advance past the symbols
		len = 2;
	}
    // This function will handle special symbols ($$, $?, and $ followed by space)
    // The logic here will depend on how you want to treat these symbols
    // For demonstration, let's just copy them as is and advance

    return (len); // Return the number of characters added to dest
}

void populateNewString(t_shell *shell, TokenNode *node, char *newStr) {
    char *value = node->token.value;
    char *p = value; // Pointer to iterate through the original string
    char *dest = newStr; // Destination pointer for the new string

    while (*p) {
        if (*p == '$') {
            if (*(p+1) == '$' || *(p+1) == '?' || isspace(*(p+1))) {
                dest += fillNewStringWithSpecialSymbols(shell, &p, dest);
            } else {
                dest += fillNewStringWithEnvVar(&p, dest, shell);
            }
        } else {
            *dest++ = *p++; // Copy regular character and advance
        }
    }
	// ft_printf("transformed string: |%s|\n", newStr);
}

void replaceTokenNode(t_shell *shell, TokenNode *oldNode, char *newStr) {
    // Create a new TokenNode with the new string
    TokenNode *newNode = (TokenNode *)shell_malloc(shell, sizeof(TokenNode));
    if (!newNode) {
        perror("Failed to allocate memory for new TokenNode");
        return;
    }
    newNode->token.value = newStr; // Assign the new string to the node
    newNode->token.type = TOKEN_ARG; // to change later, needs to be parsed to get the right argument
    // newNode->token.type = oldNode->token.type; // Copy the type from the old node

    // Special handling if oldNode is the head of the list
    if (shell->token_head == oldNode) {
        newNode->next = oldNode->next; // The new node takes over the position of the old node
        shell->token_head = newNode; // Update the head of the list to be the new node
    } else {
        // Find the previous node to oldNode
        TokenNode *prevNode = shell->token_head;
        while (prevNode != NULL && prevNode->next != oldNode) {
            prevNode = prevNode->next;
        }

        if (prevNode != NULL) {
            prevNode->next = newNode; // Link the previous node to the new node
            newNode->next = oldNode->next; // The new node points to the next node in the old node's place
        }
    }

    // At this point, oldNode is effectively replaced by newNode in the list.
    // Note: The old node is now disconnected but not freed, as required.
}




void processDQToken(t_shell *shell, TokenNode *node) {
    char *value = node->token.value;
    int newLength = 0;
    char *p = value;

    while (*p) {
        if (*p == '$') {
            if (*(p+1) == '$' || *(p+1) == '?' || isspace(*(p+1))) {
                newLength += handleSpecialSymbols(shell, &p);
            } else {
                newLength += handleEnvVar(&p);
            }
        } else {
            newLength++; // Regular character, just add to length
            p++; // Move to next character
        }
    }

    // printf("Calculated new length: %d\n", newLength);
	// (void)shell;
	// After calculating new length
	char *newStr = (char *)shell_malloc(shell, newLength + 1); // +1 for the null terminator
	if (!newStr) {
		perror("Memory allocation failed");
		return; // Exit the function or handle the error as appropriate
	}
	newStr[newLength] = '\0'; // Ensure the string is null-terminated
	populateNewString(shell, node, newStr);
	replaceTokenNode(shell, node, newStr);
}





void refine_tokens(t_shell *shell)
{
    TokenNode *current = shell->token_head;
    // TokenNode *prev = NULL;
    while (current != NULL)
	{
        if (current->token.type == TOKEN_D_Q)
            processDQToken(shell, current);
        // prev = current;
        current = current->next;
    }
}

