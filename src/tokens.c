/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:24:32 by toto              #+#    #+#             */
/*   Updated: 2024/02/26 15:48:04 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void addToken(t_shell *shell, const char *value, int type)
{
    // ft_printf("addToken shell_malloc\n");
    TokenNode *newNode = (TokenNode *)shell_malloc(shell, sizeof(TokenNode));
    if (!newNode) return;

    // ft_printf("addToken shell_strdup\n");
    newNode->token.value = shell_strdup(shell, value); // Use shell_strdup
    newNode->token.type = type;
    // ft_printf("new %d\n", newNode->token.type);
    newNode->next = NULL;

    // ft_printf("addToken token_head NULL?\n");
    if (shell->token_head == NULL)
        shell->token_head = newNode;
    else
    {
        // ft_printf("addToken token_head NULL? else\n");
        TokenNode *current = shell->token_head;
        // ft_printf("addToken while (current->next != NULL)\n");
        while (current->next != NULL)
        {
            // ft_printf("addToken while\n");
            current = current->next;
        }
        // ft_printf("addToken current->next = newNode;\n");
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
    } else if (ft_strcmp(token_text, "<<") == 0) {
        return TOKEN_REDIR_HEREDOC;
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

void set_first_token_to_command(t_shell *shell)
{
    // ft_printf("set_first_token_to_command START\n");
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
    // ft_printf("process_token START\n");
    size_t tokenLength = tokenEnd - tokenStart;
    if (tokenLength == 0) return; // Skip empty tokens

    // ft_printf("process_token allocate_and_copy_token\n");
    char *tokenValue = allocate_and_copy_token(tokenStart, tokenLength);
    if (!tokenValue) return; // Allocation check

    // TokenType type = quotetypeptr ? TOKEN_D_Q : get_token_type(tokenValue);
    // ft_printf("process_token get_quote_status\n");
    TokenType type = get_quote_status(quotetypeptr, tokenValue);
    // here deal with heredoc
    // check if previous one is a heredoc -> cant if list is not double linked or ID identifiable

    // ft_printf("token type %d\n", type);
    // ft_printf("process_token addToken %d\n", shell->token_head->);
    addToken(shell, tokenValue, type);
    ft_printf("process_token: new token |%s|\n", tokenValue);
    parse_heredoc(shell);
    // addToken(shell, head, tokenValue, type);
    free(tokenValue); // Clean up after adding the token
    tokenValue = NULL;
    // ft_printf("process_token END\n");
}


// New helper function to find the next quote of the same type
char *find_next_quote(const char *s, char quoteType) {
    while (*s && *s != quoteType) s++;
    // ft_printf("find_next_quote END\n");
    return (*s == quoteType) ? (char *)s : NULL;
}


const char *find_token_end_and_adjust_start(const char **s, char c, char **nextQuote, int *quotetypeptr)
{
    // ft_printf("find_token_end_and_adjust_start START\n");
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
    // ft_printf("find_token_end_and_adjust_start END\n");
    return end;
}



void create_tokens(t_shell *shell, const char *s)
{
    // ft_printf("create tokens ft_strlen |%s| %d\n", s, ft_strlen(s));
    while (*s) {
        s = skip_delimiters(s, ' ');

        if (!*s) break;

        char *nextQuote = NULL;
        int quotetypeptr = 0;
        const char *tokenStart = s;
        // ft_printf("1. create tokens tokenstart |%s|\n", tokenStart);
        const char *tokenEnd = find_token_end_and_adjust_start(&tokenStart, ' ', &nextQuote, &quotetypeptr);
        // ft_printf("2. create tokens tokenstart |%s|\n", tokenStart);
        process_token(shell, tokenStart, tokenEnd, quotetypeptr);

        s = nextQuote ? nextQuote + 1 : tokenEnd;
        if (!nextQuote && *s) s++;
    }
    expand_variables(shell);
    set_first_token_to_command(shell);
}

