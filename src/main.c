/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:50:11 by toto              #+#    #+#             */
/*   Updated: 2024/02/24 15:28:03 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void process_input_into_commands(int fd, t_shell *shell,t_in in)
{
	char	**line;

	line = (char**)malloc(sizeof(char*));
    while ((old_get_next_line(fd, line)) == 1)
    {
        create_tokens(shell, *line);
        // ft_printf("process_input_into_commands: line = |%s|\n", *line);
        free(*line);
    }
    free (line);
    CommandTable *command_table = create_command_table(shell, shell->token_head);
    // printTokens(shell->token_head);
    // print_command_table(command_table);
    execute_command_table(shell, command_table,in);
}

void    process_args_input(t_shell *shell, int argc, char **argv,t_in in)
{
    char    **line;
    int     fd;
    
    if (ft_strcmp(argv[1], "-c") == 0)
    {
        if (argv[2] && argc == 3)
        {
            create_tokens(shell, argv[2]);
            CommandTable *command_table = create_command_table(shell, shell->token_head);
            execute_command_table(shell, command_table,in);
        }
        else
        {
            perror("bash: option requires an argument\n");
            return ;
        }
    }
    else
    {
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
        {
            perror("error reading file\n");
            return ;
        }
        line = (char**)malloc(sizeof(char*));
        while ((old_get_next_line(fd, line)) == 1)
        {
            create_tokens(shell, *line);
            free(*line);
        }
        free (line);
        // After all lines are read and tokenized, create the command table
        CommandTable *command_table = create_command_table(shell, shell->token_head);
        // printTokens(shell->token_head);
        // print_command_table(command_table);
        execute_command_table(shell, command_table,in);
    }
}


int main(int argc, char **argv, char **envp)
{
    char* input;
    t_shell *shell;
    t_in in;
    in.argv = argv;
	in.envp = envp;
	in.argc = argc;

    shell = initialize_shell(envp);
    if (shell->is_interactive && argc == 1)
    {
        while (1)
        {
            input = readline("$ ");
            if (input == NULL) {
                ft_printf("exit\n");
                break ;
            }
            if (ft_strlen(input) > 0)
            {
                add_history(input);

                create_tokens(shell, input);
                CommandTable* command_table = create_command_table(shell, shell->token_head);
                // printTokens(shell->token_head);
                execute_command_table(shell, command_table,in);
                // print_command_table(command_table);

                shell->token_head = NULL;
                free(input);
            } else
                free(input); // Consider adjusting to use shell's memory management
        }
    }
    else
    {
        if (argc > 1)
        {
            // Non-interactive mode but with arguments
            // ft_printf("Non-interactive mode with arguments: Execute command '%s'\n", argv[1]);
            process_args_input(shell, argc, argv,in);
        } else
        {
            // Non-interactive mode due to input or output redirection
            // ft_printf("Non-interactive mode\n");
            process_input_into_commands(STDIN_FILENO, shell,in);

            // If you were handling input from a file or pipe, process it here.
            // Since we're simulating, we'll not enter a read loop.
            // This is where the program would normally exit after handling the command or input.
        }
    }

    shexit(shell, 10);
    return 0;
}
