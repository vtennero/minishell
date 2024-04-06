/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_set_cmds.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:13:35 by root              #+#    #+#             */
/*   Updated: 2024/04/06 18:43:17 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_only_spaces(char *str)
{
	int	length;

	length = ft_strlen(str);
    if ((str[0] == '\'' && str[length - 1] == '\'') || (str[0] == '"' && str[length - 1] == '"')) {
        int i = 1;

        while (i < length - 1) {
            if (!isspace(str[i])) {
                return 1; // Non-space character found
            }
            i++;
        }

        return 0;
    }

    return 1;
}

// int		check_if_valid_cmd(TokenNode *node)
// {
// 	if (!ft_strlen(node->token.value))
// 		return (0);
// 	if (is_only_spaces(node->token.value))
// 		return (0);
// 	return (1);
// }


// int is_valid_cmd(t_shell* shell,char* cmd_name)
// {
// 	char	**paths;
// 	char	*cmd_path;

// 	paths = find_cmd_paths(shell->envp);
// 	cmd_path = locate_cmd(paths, cmd_name);
// 	free_array(paths);
// 	// ft_putstr_fd(ft_strjoin_nconst("cmd path is ",cmd_path),2);
// 	if (cmd_path!=NULL && is_directory(cmd_path)!=1)
// 		return 1;
// 	else
// 		return 0;
// }

void set_token_commands(t_shell *shell)
{
    TokenNode *node = shell->token_head;
    int pipe_exist = 1;

    while (node)
	{
        if (node)
		{
			if (pipe_exist && is_valid_cmd(shell, node->token.value))
			{
                node->token.type = TOKEN_COMMAND;
                pipe_exist = 0;
            }
            // node->token.type = TOKEN_COMMAND;
            if (node->next && node->next->token.type == TOKEN_PIPE)
                pipe_exist = 1;
        }
		// if node
        node = node->next;
    }
}
