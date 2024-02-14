/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:24:32 by toto              #+#    #+#             */
/*   Updated: 2024/02/14 18:47:18 by toto             ###   ########.fr       */
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

// void addToken(TokenNode** head, Token token)
// {
//     // Create a new node
//     TokenNode* newNode = (TokenNode*)malloc(sizeof(TokenNode));
//     if (newNode == NULL) {
//         // Handle memory allocation failure
//         return;
//     }
//     newNode->token = token;  // Copy the token into the node
//     newNode->next = NULL;

//     // If the list is empty, make the new node the head
//     if (*head == NULL) {
//         *head = newNode;
//     }
// 	else
// 	{
//         // Otherwise, find the end of the list and add the new node there
//         TokenNode* current = *head;
//         while (current->next != NULL) {
//             current = current->next;
//         }
//         current->next = newNode;
//     }
// }

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
        ft_printf("Token: %s, Type: %d\n", current->token.value, current->token.type);
        current = current->next;
    }
}

// static char* ft_strsplit_strdup(const char *src, char c) {
//     char    *wd;
//     int     l;
//     int     i;

//     l = 0;
//     while (src[l] && src[l] != c)
//         l++;
//     wd = malloc(l * sizeof(char) + 1);
//     if (!wd)
//         return (NULL);
//     i = 0;
//     while (i < l) {
//         wd[i] = src[i];
//         i++;
//     }
//     wd[i] = '\0';
//     return (wd);
// }

// void ft_split_to_list(char const *s, char c, TokenNode** head)
// {
//     char    *tokenStr;
//     Token   token;

//     if (!s)
//         return;

//     while (*s)
// 	{
//         while (*s == c)
//             s++;
//         if (*s && *s != c) {
//             tokenStr = ft_strsplit_strdup(s, c);
//             token.value = tokenStr;
//             token.type = 0; // Set the type appropriately based on your logic or requirements
//             addToken(head, token);
//             // free(tokenStr); // Don't forget to free the duplicated string after adding the token
//         }
//         while (*s && *s != c)
//             s++;
//     }
// 	printTokens(*head);
// }

// working
// static char* ft_strsplit_strdup(const char *src, char c, int *advance) {
//     const char *end;
//     const char *original_src = src;
//     int inside_quotes = 0;

//     // Skip initial delimiter if it's not part of a quoted string
//     if (*src == c && *src != '\'') src++;

//     // Find the end of the token
//     for (end = src; *end; end++) {
//         if (*end == '\'' && !inside_quotes) {
//             inside_quotes = !inside_quotes; // Toggle state
//             src++; // Move start past the opening quote for the token content
//             continue;
//         } else if (*end == '\'' && inside_quotes) {
//             inside_quotes = !inside_quotes; // Toggle state
//             break; // Include quoted content as a single token
//         } else if (*end == c && !inside_quotes) {
//             break; // Stop at delimiter if not in quotes
//         }
//     }

//     // Calculate token length
//     int length = end - src + (inside_quotes ? 1 : 0);

//     // Copy the token
//     char *token = malloc(length + 1);
//     if (!token) return NULL;
//     strncpy(token, src, length);
//     token[length] = '\0';

//     // Calculate advance
//     *advance = (end - original_src) + (inside_quotes ? 0 : 1); // Adjust advance based on whether we're inside quotes

//     return token;
// }

// void ft_split_to_list(char const *s, char c, TokenNode** head) {
//     int advance;

//     while (*s) {
//         if (*s == c) {
//             s++; // Skip delimiters
//             continue;
//         }

//         char *tokenStr = ft_strsplit_strdup(s, c, &advance);
//         if (!tokenStr) break; // In case of allocation failure or end of string

//         Token token = {.value = tokenStr, .type = 0}; // Placeholder type
//         addToken(head, token);

//         s += advance; // Advance the pointer based on the token length
//     }
// 	printTokens(*head);
// }




void ft_split_to_list(const char *s, char c, TokenNode **head) {
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
            addToken(head, tokenValue, 0); // Assuming a simple type=0 for all tokens

            free(tokenValue); // Clean up
        }

        if (!nextQuote && *s) s++; // If not ending with a quote, move over the delimiter
    }
	printTokens(*head);
}





