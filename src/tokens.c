/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 22:49:02 by cliew             #+#    #+#             */
/*   Updated: 2024/04/08 22:49:02 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special(const char *str)
{
	if (ft_strncmp(str, "|", 1) == 0)
		return (1);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (2);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (2);
	else if (ft_strncmp(str, ">", 1) == 0)
		return (1);
	else if (ft_strncmp(str, "<", 1) == 0)
		return (1);
	return (0);
}

int	isvalidvarstartchar(char c)
{
	return (ft_isalpha(c) || (c == '_'));
}

int	isvalidvarchar(char c)
{
	return (ft_isalnum(c) || (c == '_'));
}

int	check_first_var_char(char *var, int *baselen)
{
	if (var[1] == '\"')
	{
		*baselen = 2;
		return (0);
	}
	else if (!var[1] || isspace_not_eol(var[1] || var[1] == '\"'
			|| var[1] == '$'))
		return (0);
	else if (var[1] == '?')
	{
		*baselen = 2;
		return (0);
	}
	else if (isvalidvarstartchar(var[1]))
		(*baselen)++;
	return (1);
}

int	check_susbsequent_chars(char *var, int *baselen)
{
	int	index;

	index = 1;
	while (var[index])
	{
		if (isvalidvarchar(var[index]))
			(*baselen)++;
		else if (var[index] == '?')
		{
			(*baselen)++;
			break ;
		}
		else
			break ;
		index++;
	}
	return (0);
}

int	get_non_expanded_var_length(char *var)
{
	int	index;
	int	baselen;
	int	result;

	index = 1;
	baselen = 1;
	result = check_first_var_char(var, &baselen);
	if (result == 0)
		return (baselen);
	index++;
	result = check_susbsequent_chars(var, &baselen);
	return (baselen);
}


char	*quotevar(t_shell *shell, const char **s)
{
	char	*result;

	result = shell_strdup(shell, "");
	if (!is_special(*s))
		result = process_quoting(shell, s, result);
	else if (is_special(*s))
	{
		result = shell_strndup(shell, (*s), is_special(*s));
		*s += is_special(*s);
	}
	return (result);
}

char	*parse_tokens(t_shell *shell, const char *s)
{
	char	*wvarexpanded;
	int		type;
	int		index;
	int		after_redirect;

	index = 0;
	after_redirect = 0;
	while (*s)
	{
		s = skip_delimiters(s, ' ');
		if (!*s)
			break ;
		wvarexpanded = quotevar(shell, &s);
		type = get_token_type(wvarexpanded);
		if (index == 0 && !is_redirect(type, &after_redirect))
			type = TOKEN_COMMAND;
		add_token(shell, wvarexpanded, type);
		index++;
		s = skip_delimiters(s, ' ');
	}
	return (NULL);
}

void	add_token(t_shell *shell, const char *value, int type)
{
	TokenNode	*new_node;
	TokenNode	*current;

	new_node = (TokenNode *)shell_malloc(shell, sizeof(TokenNode));
	if (!new_node)
		return ;
	new_node->token.value = shell_strdup(shell, value);
	new_node->token.type = type;
	new_node->next = NULL;
	if (shell->token_head == NULL)
		shell->token_head = new_node;
	else
	{
		current = shell->token_head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

TokenType	get_token_type(const char *token_text)
{
	if (ft_strcmp(token_text, "<") == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strcmp(token_text, ">") == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strcmp(token_text, ">>") == 0)
		return (TOKEN_REDIR_APPEND);
	else if (ft_strcmp(token_text, "<<") == 0)
		return (TOKEN_REDIR_HEREDOC);
	else if (ft_strcmp(token_text, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(token_text, "$?") == 0)
		return (TOKEN_EXIT_STATUS);
	return (TOKEN_ARG);
}



void	create_tokens(t_shell *shell, const char *s)
{
	parse_tokens(shell, s);
	set_token_commands(shell);
}
