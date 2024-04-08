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

int	isSpecialOperator(const char *str)
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

int	is_redirect(int type, int *after_redirect)
{
	if ((type == 2) || (type == 3) || (type == 4) || (type == 5))
	{
		*after_redirect = 1;
		return (1);
	}
	else
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
	// while (var[index])
	// {
	result = check_first_var_char(var, &baselen);
	if (result == 0)
		return (baselen);
	// break ;
	index++;
	// }
	result = check_susbsequent_chars(var, &baselen);
	return (baselen);
}

// int	get_non_expanded_var_length1(char *var)
// {
// 	int count = 1;       // Counter for valid characters
// 	int index = 0;       // Current index in the string
// 	int foundLetter = 0; // Flag to track if at least one letter has been found
// 	int foundDol = 1;    // Flag to track if at least one letter has been found
// 	var++;
// 	while (var[index] != '\0')
// 	{
// 		if (var[index] == '$')
// 			foundDol++;
// 		else if (!ft_isalpha(var[index]) && !ft_isdigit(var[index])
// 			&& var[index] != '?')
// 		{
// 			break ;
// 		}
// 		else if (var[index] == '?' && var[index + 1] == '\"')
// 			return (2);
// 		else if (ft_isalpha(var[index]))
// 		{
// 			foundLetter = 1; // Set the flag if a letter is found
// 		}
// 		count++; // Increment the count of valid characters
// 		index++; // Move to the next character
// 	}
// 	if (foundDol == count)
// 		return (count);
// 	else if (!foundLetter)
// 		count = 1;
// 	return (count);
// }



char	*quotevar(t_shell *shell, const char **s)
{
	char	*result;

	result = shell_strdup(shell, "");
	if (!isSpecialOperator(*s))
	{
		result = process_quoting(shell, s, result);
	}
	else if (isSpecialOperator(*s))
	{
		result = shell_strndup(shell, (*s), isSpecialOperator(*s));
		*s += isSpecialOperator(*s);
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
	TokenNode	*newNode;
	TokenNode	*current;

	newNode = (TokenNode *)shell_malloc(shell, sizeof(TokenNode));
	if (!newNode)
		return ;
	newNode->token.value = shell_strdup(shell, value); // Use shell_strdup
	newNode->token.type = type;
	newNode->next = NULL;
	if (shell->token_head == NULL)
		shell->token_head = newNode;
	else
	{
		current = shell->token_head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newNode;
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

int	check_if_valid_cmd(TokenNode *node)
{
	if (!ft_strlen(node->token.value))
		return (0);
	if (is_only_spaces(node->token.value))
		return (0);
	return (1);
}

int	is_valid_cmd(t_shell *shell, char *cmd_name)
{
	char	**paths;
	char	*cmd_path;
	int		is_custom_cmd;

	is_custom_cmd = 0;
	if ((ft_strcmp(cmd_name, "cd") == 0) || (ft_strcmp(cmd_name, "env") == 0)
		|| (ft_strcmp(cmd_name, "exit") == 0) || (ft_strcmp(cmd_name,
				"unset") == 0) || (ft_strcmp(cmd_name, "export") == 0)
		|| (ft_strcmp(cmd_name, "echo") == 0) || (ft_strcmp(cmd_name,
				"pwd") == 0))
		is_custom_cmd = 1;
	paths = find_cmd_paths(shell->envp);
	cmd_path = locate_cmd(paths, cmd_name);
	free_array(paths);
	// ft_putstr_fd(ft_strjoin_nconst("cmd path is ",cmd_path),2);
	if ((cmd_path != NULL && is_directory(cmd_path) != 1) || is_custom_cmd == 1)
		return (1);
	else
		return (0);
}

void	set_commands_check(t_shell *shell, TokenNode *node, int *after_redirect,
		int *pipe_exist)
{
	if (node && isNotEmpty(node->token.value))
	{
		if (!*after_redirect && (node == shell->token_head))
		{
			node->token.type = TOKEN_COMMAND;
			*pipe_exist = 0;
		}
		else if (*pipe_exist && !*after_redirect
			&& node->token.type != TOKEN_PIPE && node->token.type != TOKEN_PIPE)
		{
			node->token.type = TOKEN_COMMAND;
			*pipe_exist = 0;
		}
		if (node->token.type == TOKEN_PIPE)
			*pipe_exist = 1;
	}
}
void	set_token_commands(t_shell *shell)
{
	TokenNode	*node;
	int			pipe_exist;
	int			after_redirect;

	node = shell->token_head;
	pipe_exist = 1;
	after_redirect = 0;
	while (node)
	{
		if (!check_if_valid_cmd(node) && (node == shell->token_head))
			node->token.type = TOKEN_INV_COMMAND;
		else
		{
			while (is_redirect(node->token.type, &after_redirect) && node->next
				&& node->next->next)
			{
				node = node->next->next;
				after_redirect = 0;
			}
		}
		set_commands_check(shell, node, &after_redirect, &pipe_exist);
		node = node->next;
	}
}

void	create_tokens(t_shell *shell, const char *s)
{
	parse_tokens(shell, s);
	set_token_commands(shell);
}
