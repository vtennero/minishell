/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_set_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:13:35 by root              #+#    #+#             */
/*   Updated: 2024/04/10 17:04:46 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	is_only_spaces(char *str)
{
	int	length;
	int	i;

	length = ft_strlen(str);
	if ((str[0] == '\'' && str[length - 1] == '\'') || (str[0] == '"' && str[length - 1] == '"')) {
		i = 1;

		while (i < length - 1) {
			if (!ft_isspace(str[i])) {
				return 0;
			}
			i++;
		}

		return 1;
	}

	return 0;
}

int	check_if_valid_cmd(t_token_node *node)
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
	if ((cmd_path != NULL && is_directory(cmd_path) != 1) || is_custom_cmd == 1)
		return (1);
	else
		return (0);
}

void	set_commands_check(t_shell *shell, t_token_node *node, int *after_redirect,
		int *pipe_exist)
{
	if (node && not_empty(node->token.value))
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
	t_token_node	*node;
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

