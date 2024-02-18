/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:24:32 by toto              #+#    #+#             */
/*   Updated: 2024/02/18 16:13:56 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void freeTokenList(TokenNode* head)
{
    TokenNode* current = head;
    while (current != NULL) {
        TokenNode* next = current->next;
        // Free the current node
        free(current);
        current = next;
    }
	current = NULL;
}


void addToken(TokenNode **head, const char *value, int type)
{
    TokenNode *newNode = (TokenNode *)malloc(sizeof(TokenNode));
    if (!newNode) return;

    newNode->token.value = strdup(value); // Ensure value is copied
    newNode->token.type = type;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        TokenNode *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void printTokens(TokenNode* head)
{
    TokenNode* current = head;
    while (current != NULL) {
        ft_printf("Token: %s \t | Type: %d\n", current->token.value, current->token.type);
        current = current->next;
    }
}

// Helper function to determine the type of a single token
TokenType get_token_type(const char* token) {
    if (strcmp(token, "<") == 0) {
        return TOKEN_REDIR_IN;
    } else if (strcmp(token, ">") == 0) {
        return TOKEN_REDIR_OUT;
    } else if (strcmp(token, ">>") == 0) {
        return TOKEN_REDIR_APPEND;
    } else if (strcmp(token, "|") == 0) {
        return TOKEN_PIPE;
    } else if (token[0] == '$') {
        if (strcmp(token, "$?") == 0) {
            return TOKEN_EXIT_STATUS;
        } else {
            return TOKEN_ENV_VAR;
        }
    }
    // This is a simplified heuristic: we assume any token not recognized as a special
    // symbol or variable is a command or argument. A more complex shell might need
    // additional logic to distinguish between commands and arguments based on context.
    return TOKEN_ARG; // Default case, can be TOKEN_COMMAND or TOKEN_ARG based on context
}

void set_first_token_to_command(TokenNode** head) {
    if (head && *head) { // Check if the head is not NULL and points to a valid node
        (*head)->token.type = TOKEN_COMMAND; // Set the type of the first token to TOKEN_COMMAND
    }
}

void ft_split_to_list(const char *s, char c, TokenNode **head)
{
    while (*s) {
        while (*s == c) s++; // Skip leading delimiters

        if (!*s) break; // End of string check

        char *nextQuote = NULL;
        if (*s == '\'') {
            nextQuote = strchr(s + 1, '\''); // Look for the closing quote
        }

        const char *tokenStart = s;
        const char *tokenEnd = s;

        if (nextQuote) {
            tokenStart = s + 1; // Start after the opening quote
            tokenEnd = nextQuote; // End at the closing quote
            s = nextQuote + 1; // Move past the closing quote for next iteration
        } else {
            // Find the next delimiter or end of string for unquoted tokens
            while (*tokenEnd && *tokenEnd != c) tokenEnd++;
            s = tokenEnd; // Prepare for next iteration
        }

        // Calculate token length
        size_t tokenLength = tokenEnd - tokenStart;
        if (tokenLength > 0) {
            // Allocate and copy the token
            char *tokenValue = strndup(tokenStart, tokenLength);
            if (!tokenValue) break; // Allocation check

            // Add the token
            addToken(head, tokenValue, get_token_type(tokenValue));

            free(tokenValue); // Clean up
        }

        if (!nextQuote && *s) s++; // If not ending with a quote, move over the delimiter
    }
    set_first_token_to_command(head);
    process_pipes(head);

	// printTokens(*head);
}





