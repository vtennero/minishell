/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:45:10 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/06 18:32:44 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interactive_mode(t_shell *shell)
{
	char			*input;
	char			*input2;
	char			*temp;
	CommandTable	*command_table;

	input2 = NULL;
	while (1)
	{
		input = readline("$ ");
		if (input == NULL)
		{
			ft_printf("exit\n");
			break ;
		}
		else if (end_with_pipe(input) == 1 && !isNotEmpty(input2))
		{
			input2 = readline("> ");
			if (input == NULL)
			{
				ft_printf("exit\n");
				break ;
			}
		}
		if (input2)
		{
			temp = ft_strjoin_nconst(input, input2);
			free(input);
			input = shell_strdup(shell, temp);
			free(temp);
		}
		input2 = "";
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			create_tokens(shell, input);
			command_table = create_command_table(shell, shell->token_head);
			printTokens(shell->token_head);
			print_command_table(command_table);
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

/*
Non-interactive mode but with arguments
*/
void	w_arg_mode(t_shell *shell, int argc, char **argv)
{
	if (ft_strcmp(argv[1], "-c") == 0)
		w_arg_mode_c(shell, argv);
	else
		w_arg_mode_read_file(shell, argc, argv);
}

/*
Non-interactive mode due to input or output redirection
*/
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
