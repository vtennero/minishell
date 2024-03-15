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
	// ft_printf("addToken new token type is %d\n", newNode->token.type);
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
	} else if (ft_strcmp(token_text, "$?") == 0)
		return TOKEN_EXIT_STATUS;
	return TOKEN_ARG; // Default case, can be TOKEN_COMMAND or TOKEN_ARG based on context
}



char    *parse_tokens(t_shell *shell, const char *s)
{
	char    *processedQuotes;
	char    *wvarexpanded;
	int     type;
	int     isInSingleQuotes = 0;
	int		isInDoubleQuotes = 0; // Track quote states
	int		index;

	index = 0;
	while (*s)
	{
		s = skip_delimiters(s, ' ');
		if (!*s)
			break;
		const char *start = s; // Start of the word
		// while (*s && !(isspace(*s)) || isInSingleQuotes || isInDoubleQuotes)
		while (*s && (!isspace(*s) || isInSingleQuotes || isInDoubleQuotes))
		{
			if (*s == '\'' && !isInDoubleQuotes) {
				toggleQuoteState(&isInSingleQuotes);
			} else if (*s == '\"' && !isInSingleQuotes) {
				toggleQuoteState(&isInDoubleQuotes);
			}
			s++;
		}

		int wordLength = s - start;
		char *word = strndup(start, wordLength); // Copy the current word
		processedQuotes = reviewquotes(strdup(word));
		wvarexpanded = expandVariables(shell, processedQuotes, shell->env_head);
		// ft_printf("with quotes expanded |%s|\n", processedQuotes);
		// ft_printf("with variables expanded  |%s|\n", wvarexpanded);
		if (index == 0)
			type = 0;
		else
			type = get_token_type(wvarexpanded);
		addToken(shell, wvarexpanded, type);
		index++;
		parse_heredoc(shell);
		s = skip_delimiters(s, ' ');
		// while (*s == ' ')
			// s++;
	}
	return (NULL);
}

void    set_commands(t_shell *shell)
{
	TokenNode *node = shell->token_head;
	if (node){
		node->token.type=TOKEN_COMMAND;

		while (node->next)
			{
				if (node->token.type == TOKEN_PIPE)
				{
					node=node->next;
					node->token.type=TOKEN_COMMAND;
				}
				else
					node=node->next;
			}

	}

}

void    createtokensv2(t_shell *shell, const char *s)
{
	// ft_printf("createtokensv2 START\n");
	// printf("str ois %s \n",s);
	parse_tokens(shell, s);
	// printTokens(shell->token_head);
	// printTokens(shell->token_head);
	set_commands(shell);
 
	// ft_printf("createtokensv2 END\n");

}

void create_tokens(t_shell *shell, const char *s)
{
	createtokensv2(shell, s);
}