/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:45:10 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/13 09:28:10 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_tokens( t_token_node *node)
 {
	
	t_token_node* temp;
	int empty;
	// int count;
	temp = node;

	// count = 0;


	while (temp && temp != NULL)
	{
		if (temp->token.type==6 && temp->next && temp->next->token.type==6 )
			return 1;
		if (is_redirect(temp->token.type, &empty) && temp->next && is_redirect(temp->next ->token.type, &empty))
			return 1;
		if (is_redirect(temp->token.type, &empty) && !temp->next )
			return 1;
		
		temp = temp->next;
		// count++;
	}
	return 0;
	
}
void	interactive_mode(t_shell *shell)
{
	char			*input;
	t_cmd_table		*command_table;

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
			print_tokens(shell->token_head);

		}
		if (!check_tokens(shell->token_head))
		{
			command_table = create_command_table(shell, shell->token_head);
			print_command_table(command_table);

			execute_command_table(shell, command_table);
		}
		else 
		{
			ft_putstr_fd("syntax error near unexpected token \n", 2);
			// shell_cleanup(shell);
		}
		shell->token_head = NULL;

		
	}
}

void	w_arg_mode_c(t_shell *shell, char **argv)
{
	char			**line;
	int				fd;
	t_cmd_table		*command_table;

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

void	w_arg_mode_read_file(t_shell *shell, char **argv)
{
	t_cmd_table		*command_table;
	char			**line;
	int				fd;

	fd = open(argv[1], O_RDONLY);
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
		ft_putstr_fd("bash: option requires an argument\n", 2);
}

void	w_arg_mode(t_shell *shell, int argc, char **argv)
{
	if (ft_strcmp(argv[1], "-c") == 0)
		w_arg_mode_c(shell, argv);
	else
		w_arg_mode_read_file(shell, argv);
	(void)argc;
}

void	std_input_mode(int fd, t_shell *shell)
{
	char			**line;
	t_cmd_table		*command_table;

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


