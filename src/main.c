/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:50:11 by toto              #+#    #+#             */
/*   Updated: 2024/02/21 16:57:42 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// signal handler
void setup_signals(t_shell *shell)
	{
    // Signal handling setup
    // Set up signal handlers
    signal(SIGINT, sigint_handler);  // Handle Ctrl-C
    signal(SIGQUIT, sigquit_handler); // Handle Ctrl-\, do nothing
    // Any other initialization
    (void)shell;
}

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

void process_input_into_commands(int fd, t_shell *shell) {
    char *input;

    // ft_printf("Non-interactive mode with input or output redirection.\n");
    // Loop to read each line of input using get_next_line
    while ((input = get_next_line(fd)) != NULL) {
        // Tokenize the input line
        create_tokens(shell, input);

        // Free the input line after tokenizing it
        free(input);
    }

    // After all lines are read and tokenized, create the command table
    CommandTable *command_table = create_command_table(shell, shell->token_head);
    // printTokens(shell->token_head);
    // print_command_table(command_table);
    execute_command_table(shell, command_table);

    // Assuming create_command_table also links commands, the command_table is now ready to use
    // Here you could execute the commands, print them for debug, etc.

    // Clean up the command table and any other allocated resources if necessary
    // This part is left out as it depends on how you manage memory and structures within your shell
}

// new
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

// old, works
// int main(int argc, char **argv, char **envp)
// {
//     char* input;
//     TokenNode* head = NULL;
//     char **arg_input;
//     int i;

//     // Initialize your shell and get the shell structure
//     int interactive_mode = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);

//     ft_printf("interactive mode? %d\n", interactive_mode);
    
//     t_shell* shell = initialize_shell(envp);
//     (void)envp;
//     if (argc == 3 && ft_strcmp(argv[1], "-c") == 0 && argv[2])
//     {
//         ft_printf("-c mode\n");
//         arg_input = ft_split(argv[2], ';'); //this leaks 
//         if (!arg_input)
//             shexit(shell, 1); // Assuming shexit now also takes a shell pointer for cleanup

//         i = 0;
//         while (arg_input[i])
//         {
//             // ft_printf("non interactive mode\n");
//             // Parse and execute arg_input[i], pass shell as needed
//             i++;
//         }
//     }
//     else
//     {
//         ft_printf("interactive mode\n");
//         while (1)
//         {
//             input = readline("$ ");
//             if (input == NULL) {
//                 ft_printf("exit\n");
//                 break ;
//             }
//             if (ft_strlen(input) > 0)
//             {
//                 add_history(input);

//                 create_tokens(shell, input);
//                 CommandTable* command_table = create_command_table(shell, shell->token_head);
//                 printTokens(shell->token_head);
//                 print_command_table(command_table);
//                 (void)head;

//                 execute_command_table(shell, command_table);

//                 shell->token_head = NULL;
//                 head = NULL;
//                 free(input);
//             } else
//                 free(input); // Consider adjusting to use shell's memory management
//         }
//     }
//     shexit(shell, 0); // Adjust shexit if necessary
//     return 0;
// }



// test main
// int main(int argc, char *argv[])
// {
//     // Check if both input and output are from/to a terminal
//     int input_is_terminal = isatty(STDIN_FILENO);
//     int output_is_terminal = isatty(STDOUT_FILENO);

//     if (input_is_terminal && output_is_terminal && argc == 1)
//     {
//         // Interactive mode: No arguments, input and output are from/to a terminal.
//         printf("Interactive mode: Enter commands in a readline loop.\n");
//         perror("this is just a test error message 0\n");
//         char *line = NULL;
//         size_t len = 0;
//         printf("minishell> "); // Prompt for input
//         while (getline(&line, &len, stdin) != -1) {
//             printf("Execute: %s", line); // Simulate command execution
//             printf("minishell> "); // Prompt for input again
//             perror("this is just a test error message 1\n");

//         }
//         free(line); // Cleanup
//     } else {
//         // For non-interactive mode or when arguments are provided
//             perror("this is just a test error message 2\n");
//         if (argc > 1) {
//             // Non-interactive mode but with arguments
//             printf("Non-interactive mode with arguments: Execute command '%s'\n", argv[1]);
//         } else {
//             // Non-interactive mode due to input or output redirection
//             printf("Non-interactive mode with input or output redirection.\n");

//             // If you were handling input from a file or pipe, process it here.
//             // Since we're simulating, we'll not enter a read loop.
//             // This is where the program would normally exit after handling the command or input.
//         }
//     }
//     return 0;
// }
