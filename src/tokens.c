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

char	*parse_quotes_and_vars(t_shell *shell, const char **s)
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
		wvarexpanded = parse_quotes_and_vars(shell, &s);
		type = gee_token_type(wvarexpanded);
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
	t_token_node	*new_node;
	t_token_node	*current;

	new_node = (t_token_node *)shell_malloc(shell, sizeof(t_token_node));
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

void	create_tokens(t_shell *shell, const char *s)
{
	parse_tokens(shell, s);
	set_token_commands(shell);
}
