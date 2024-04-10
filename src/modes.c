/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:45:10 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/08 22:11:47 by cliew            ###   ########.fr       */
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

void	interactive_mode(t_shell *shell)
{
	char			*input;
	CommandTable	*command_table;

	while (1)
	{
		input = readline("$ ");
		if (input == NULL)
		{
			ft_printf("exit\n");
			break ;
		}
		if (ft_strlen(input) > 0 && pipe_extension(shell, input) != -1)
		{
			add_history(input);
			create_tokens(shell, input);
			// printTokens(shell->token_head);
			command_table = create_command_table(shell, shell->token_head);
			// print_command_table(command_table);
			execute_command_table(shell, command_table);
			shell->token_head = NULL;
		}
	}
}

void	w_arg_mode_c(t_shell *shell, char **argv)
{
	char			**line;
	int				fd;
	CommandTable	*command_table;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("error reading file\n");
		return ;
	}
	line = (char **)malloc(sizeof(char *));
	while ((old_get_next_line(fd, line)) == 1)
	{
		create_tokens(shell, *line);
		free(*line);
	}
	free(line);
	command_table = create_command_table(shell, shell->token_head);
	execute_command_table(shell, command_table);
}

void	w_arg_mode_read_file(t_shell *shell, int argc, char **argv)
{
	CommandTable	*command_table;

	if (argv[2] && argc == 3)
	{
		create_tokens(shell, argv[2]);
		command_table = create_command_table(shell, shell->token_head);
		execute_command_table(shell, command_table);
	}
	else
	{
		perror("bash: option requires an argument\n");
		return ;
	}
}

void	w_arg_mode(t_shell *shell, int argc, char **argv)
{
	if (ft_strcmp(argv[1], "-c") == 0)
		w_arg_mode_c(shell, argv);
	else
		w_arg_mode_read_file(shell, argc, argv);
}

void	std_input_mode(int fd, t_shell *shell)
{
	char			**line;
	CommandTable	*command_table;

	line = (char **)malloc(sizeof(char *));
	while ((old_get_next_line(fd, line)) == 1)
	{
		create_tokens(shell, *line);
		free(*line);
	}
	free(line);
	command_table = create_command_table(shell, shell->token_head);
	execute_command_table(shell, command_table);
}
