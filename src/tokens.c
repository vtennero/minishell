/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:24:32 by toto              #+#    #+#             */
/*   Updated: 2024/02/19 18:36:57 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void addToken(t_shell *shell, TokenNode **head, const char *value, int type)
{
    TokenNode *newNode = (TokenNode *)shell_malloc(shell, sizeof(TokenNode));
    if (!newNode) return;

    newNode->token.value = shell_strdup(shell, value); // Use shell_strdup
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
    ft_printf("\n");
    while (current != NULL) {
        ft_printf("Token: %s \t | Type: %d\n", current->token.value, current->token.type);
        current = current->next;
    }
    ft_printf("\n");
}

// Helper function to determine the type of a single token
TokenType get_token_type(const char* token_text)
{
    if (ft_strcmp(token_text, "<") == 0) {
        return TOKEN_REDIR_IN;
    } else if (ft_strcmp(token_text, ">") == 0) {
        return TOKEN_REDIR_OUT;
    } else if (ft_strcmp(token_text, ">>") == 0) {
        return TOKEN_REDIR_APPEND;
    } else if (ft_strcmp(token_text, "|") == 0) {
        return TOKEN_PIPE;
    } else if (token_text[0] == '$') {
        if (ft_strcmp(token_text, "$?") == 0) {
            return TOKEN_EXIT_STATUS;
        } else {
            ft_printf("gettokentype %s\n", token_text);
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

const char *skip_delimiters(const char *s, char c) {
    while (*s == c) s++;
    return s;
}


char *allocate_and_copy_token(const char *start, size_t length) {
    return ft_strndup(start, length);
}

void process_token(t_shell *shell, TokenNode **head, const char *tokenStart, const char *tokenEnd) {
    size_t tokenLength = tokenEnd - tokenStart;
    if (tokenLength == 0) return; // Skip empty tokens

    char *tokenValue = allocate_and_copy_token(tokenStart, tokenLength);
    if (!tokenValue) return; // Allocation check

    addToken(shell, head, tokenValue, get_token_type(tokenValue));
    free(tokenValue); // Clean up after adding the token
}

// New helper function to find the next quote of the same type
char *find_next_quote(const char *s, char quoteType) {
    while (*s && *s != quoteType) s++;
    return (*s == quoteType) ? (char *)s : NULL;
}

const char *find_token_end_and_adjust_start(const char **s, char c, char **nextQuote) {
    if (**s == '\'' || **s == '"') {
        char quoteType = **s;
        *nextQuote = find_next_quote(*s + 1, quoteType);
        if (*nextQuote) {
            *s = *s + 1; // Skip the opening quote for token start
            return *nextQuote; // Return position of closing quote
        }
    }

    const char *end = *s;
    while (*end && *end != c) end++;
    return end;
}



void create_tokens(t_shell *shell, const char *s, char c, TokenNode **head) {
    while (*s) {
        s = skip_delimiters(s, c);

        if (!*s) break;

        char *nextQuote = NULL;
        const char *tokenStart = s;
        const char *tokenEnd = find_token_end_and_adjust_start(&tokenStart, c, &nextQuote);
        process_token(shell, head, tokenStart, tokenEnd);

        s = nextQuote ? nextQuote + 1 : tokenEnd;
        if (!nextQuote && *s) s++;
    }
    set_first_token_to_command(head);
}