/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:45:10 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/31 19:34:58 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int isNotEmpty(const char *str) {
    // Check if string is NULL
    if (str == NULL)
        return 0; // String is NULL
    
    // Check if string is empty or consists of only whitespace characters
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i]))
            return 1; // String is not empty and contains non-whitespace characters
    }
    
    return 0; // String is empty or consists of only whitespace characters
}
int end_with_pipe(const char *str) {
    if (str == NULL)
        return 0; // String is NULL

    int i = 0;
    while (str[i] != '\0')
        i++; // Move to the end of the string

    // Move backwards, skipping whitespace characters
    while (i > 0 && isspace((unsigned char)str[i - 1]))
        i--;

    // Check if the last non-space character is a pipe symbol
    if (i > 0 && str[i - 1] == '|')
        return 1; // Last non-space character is a pipe symbol
    else
        return 0; // Last non-space character is not a pipe symbol
}
void	interactive_mode(t_shell *shell)
{
	char			*input;
	char			*input2=NULL;
	char* temp;
	CommandTable	*command_table;
	// int status=0;
	while (1)
	{
			input = readline("$ ");
			if (input == NULL)
			{
				ft_printf("exit\n");
				break ;
			}
			else if (end_with_pipe(input)==1 && !isNotEmpty(input2))
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
					temp =ft_strjoin_nconst(input,input2);
					free(input);
					input=shell_strdup(shell,temp);
					free(temp);
			}
			input2="";

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
			// free(input);
		// }
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
	// printTokens(shell->token_head);
	// print_command_table(command_table);
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

// Non-interactive mode but with arguments
void	w_arg_mode(t_shell *shell, int argc, char **argv)
{
	if (ft_strcmp(argv[1], "-c") == 0)
		w_arg_mode_c(shell, argv);
	else
		w_arg_mode_read_file(shell, argc, argv);
}

// Non-interactive mode due to input or output redirection
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
	// printTokens(shell->token_head);
	// print_command_table(command_table);
	execute_command_table(shell, command_table);
}



