/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externalcommands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:31:07 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/18 14:32:07 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_extension(t_shell *shell, char *input)
{
	char	*input2;
	char	*temp;

	input2 = NULL;
	while (end_with_pipe(input) == 1 && !not_empty(input2))
	{
		if (ft_strnstr(input, "||", ft_strlen(input)))
		{
			ft_putstr_fd("syntax error near unexpected token `||'", 2);
			shell->last_exit_status = 2;
			return (-1);
		}
		input2 = readline("> ");
		if (not_empty(input2))
		{
			temp = ft_strjoin_nconst(input, input2);
			free(input);
			input = shell_strdup(shell, temp);
			free(temp);
		}
		input2 = "";
	}
	return (0);
}