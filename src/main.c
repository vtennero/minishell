/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:50:11 by toto              #+#    #+#             */
/*   Updated: 2024/02/22 15:13:46 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell *initialize_shell(char **envp)
{
    t_shell* shell = (t_shell*)ft_calloc(1, sizeof(t_shell));
    if (!shell) {
        perror("Failed to initialize shell");
        exit(EXIT_FAILURE);
    }

    // Initialize the memory tracker within the shell
    shell->mem_tracker.head = NULL;
    shell->token_head = NULL;
    // change this!
    shell->last_exit_status = 780;
    shell->is_interactive = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
    // Initialize environment variables from envp, signal handlers, etc.
    // shell->env_vars = copy_env_vars(envp); // Example initialization, you'll need to implement copy_env_vars or similar
    setup_signals(shell); // Example function to setup custom signal handlers
    (void)envp;

    return shell;
}

void process_input_into_commands(int fd, t_shell *shell)
{
	char	**line;

	line = (char**)malloc(sizeof(char*));
    while ((old_get_next_line(fd, line)) == 1)
    {
        create_tokens(shell, *line);
        free(*line);
    }
    CommandTable *command_table = create_command_table(shell, shell->token_head);
    // printTokens(shell->token_head);
    // print_command_table(command_table);
    execute_command_table(shell, command_table);
}

void    process_args_input(t_shell *shell, int argc, char **argv)
{
    char    **line;
    int     fd;
    
    if (ft_strcmp(argv[1], "-c") == 0)
    {
        if (argv[2] && argc == 3)
        {
            create_tokens(shell, argv[2]);
            CommandTable *command_table = create_command_table(shell, shell->token_head);
            execute_command_table(shell, command_table);
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

        // After all lines are read and tokenized, create the command table
        CommandTable *command_table = create_command_table(shell, shell->token_head);
        // printTokens(shell->token_head);
        // print_command_table(command_table);
        execute_command_table(shell, command_table);
    }
}


int main(int argc, char **argv, char **envp)
{
    char* input;
    t_shell *shell;

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
                // print_command_table(command_table);
                execute_command_table(shell, command_table);

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
            printf("Non-interactive mode with arguments: Execute command '%s'\n", argv[1]);
            process_args_input(shell, argc, argv);
        } else {
            // Non-interactive mode due to input or output redirection
            process_input_into_commands(STDIN_FILENO, shell);

            // If you were handling input from a file or pipe, process it here.
            // Since we're simulating, we'll not enter a read loop.
            // This is where the program would normally exit after handling the command or input.
        }
    }

    shexit(shell, 0); // Adjust shexit if necessary
    return 0;
}
