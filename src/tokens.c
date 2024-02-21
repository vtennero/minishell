/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:24:32 by toto              #+#    #+#             */
/*   Updated: 2024/02/21 11:38:06 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void addToken(t_shell *shell, const char *value, int type)
{
    TokenNode *newNode = (TokenNode *)shell_malloc(shell, sizeof(TokenNode));
    if (!newNode) return;

    newNode->token.value = shell_strdup(shell, value); // Use shell_strdup
    newNode->token.type = type;
    // ft_printf("new %d\n", newNode->token.type);
    newNode->next = NULL;

    if (shell->token_head == NULL) {
        shell->token_head = newNode;
    } else {
        TokenNode *current = shell->token_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
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
            // ft_printf("gettokentype %s\n", token_text);
            return TOKEN_ENV_VAR;
        }
    }
    // This is a simplified heuristic: we assume any token not recognized as a special
    // symbol or variable is a command or argument. A more complex shell might need
    // additional logic to distinguish between commands and arguments based on context.
    return TOKEN_ARG; // Default case, can be TOKEN_COMMAND or TOKEN_ARG based on context
}

void set_first_token_to_command(t_shell *shell) {
    if (shell->token_head) 
    { // Check if the token_head points to a valid node
        shell->token_head->token.type = TOKEN_COMMAND; // Set the type of the first token to TOKEN_COMMAND
    }
}


const char *skip_delimiters(const char *s, char c) {
    while (*s == c) s++;
    return s;
}


char *allocate_and_copy_token(const char *start, size_t length) {
    return ft_strndup(start, length);
}

int get_quote_status(int quotetype, char *token_value)
{
    if (quotetype == TOKEN_D_Q)
        return(TOKEN_D_Q);
    else if (quotetype == TOKEN_S_Q)
        return (TOKEN_S_Q);
    else
        return (get_token_type(token_value));
}


void process_token(t_shell *shell, const char *tokenStart, const char *tokenEnd, int quotetypeptr)
{
    size_t tokenLength = tokenEnd - tokenStart;
    if (tokenLength == 0) return; // Skip empty tokens

    char *tokenValue = allocate_and_copy_token(tokenStart, tokenLength);
    if (!tokenValue) return; // Allocation check

    // TokenType type = quotetypeptr ? TOKEN_D_Q : get_token_type(tokenValue);
    TokenType type = get_quote_status(quotetypeptr, tokenValue);
    // ft_printf("token type %d\n", type);
    addToken(shell, tokenValue, type);
    // addToken(shell, head, tokenValue, type);
    free(tokenValue); // Clean up after adding the token
    tokenValue = NULL;
}


// New helper function to find the next quote of the same type
char *find_next_quote(const char *s, char quoteType) {
    while (*s && *s != quoteType) s++;
    return (*s == quoteType) ? (char *)s : NULL;
}


const char *find_token_end_and_adjust_start(const char **s, char c, char **nextQuote, int *quotetypeptr)
{
    *quotetypeptr = 0; // Default to not double-quoted
    if (**s == '\'' || **s == '"') {
        char quoteType = **s;
        *nextQuote = find_next_quote(*s + 1, quoteType);
        if (*nextQuote) {
            *s = *s + 1; // Skip the opening quote for token start
            // if (quoteType == '\'') { //test this 
            if (quoteType == '"') {
                *quotetypeptr = TOKEN_D_Q; // Flag as double-quoted
            }
            else if (quoteType == '\'')
                *quotetypeptr = TOKEN_S_Q; // flag as single quoted
            return *nextQuote; // Return position of closing quote
        }
    }

    const char *end = *s;
    while (*end && *end != c) end++;
    return end;
}



void create_tokens(t_shell *shell, const char *s)
{
    while (*s) {
        s = skip_delimiters(s, ' ');

        if (!*s) break;

        char *nextQuote = NULL;
        int quotetypeptr = 0;
        const char *tokenStart = s;
        const char *tokenEnd = find_token_end_and_adjust_start(&tokenStart, ' ', &nextQuote, &quotetypeptr);
        process_token(shell, tokenStart, tokenEnd, quotetypeptr);

        s = nextQuote ? nextQuote + 1 : tokenEnd;
        if (!nextQuote && *s) s++;
    }
    expand_variables(shell);
    set_first_token_to_command(shell);
}

