/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:45:10 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 15:44:09 by vitenner         ###   ########.fr       */
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
			command_table = create_command_table(shell, shell->token_head);
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
		ft_putstr_fd("error reading file\n", 2);
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

// void	w_arg_mode_read_file(t_shell *shell, int argc, char **argv)
// {
// 	CommandTable	*command_table;
// 	char			**line;
// 	int				fd;

// 	fd = open(argv[1], O_RDONLY);
// 	ft_printf("w_arg_mode_read_file %d\n", argc);
// 	ft_printf("w_arg_mode_read_file %s\n", argv[0]);
// 	ft_printf("w_arg_mode_read_file %s\n", argv[1]);
// 	(void)argc;
// 	if (argv[1])
// 	// if (argv[2] && argc == 3)
// 	{
// 		line = (char **)malloc(sizeof(char *));
// 		while ((old_get_next_line(fd, line)) == 1)
// 		{
// 			create_tokens(shell, *line);
// 			// printTokens(shell->token_head);
// 			free(*line);
// 		}
// 		free(line);
// 		create_tokens(shell, argv[1]);
// 		printTokens(shell->token_head);
// 		command_table = create_command_table(shell, shell->token_head);
// 		execute_command_table(shell, command_table);
// 	}
// 	else
// 	{
// 		ft_putstr_fd("bash: option requires an argument\n", 2);
// 		return ;
// 	}
// }

void	w_arg_mode_read_file(t_shell *shell, int argc, char **argv)
{
	CommandTable	*command_table;
	char			**line;
	int				fd;

	fd = open(argv[1], O_RDONLY);
	(void)argc;
	if (argv[1])
	{
		line = (char **)malloc(sizeof(char *));
		shell->readfile = shell_strdup(shell, argv[1]);
		while ((old_get_next_line(fd, line)) == 1)
		{
			create_tokens(shell, *line);
			command_table = create_command_table(shell, shell->token_head);
			execute_command_table(shell, command_table);
			free(*line);
			shell->token_head = NULL;
			shell->line_count++;
		}
		free(line);
	}
	else
	{
		ft_putstr_fd("bash: option requires an argument\n", 2);
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
