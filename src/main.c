/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:50:11 by toto              #+#    #+#             */
/*   Updated: 2024/02/19 15:55:37 by vitenner         ###   ########.fr       */
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

t_shell* initialize_shell(char **envp)
{
    t_shell* shell = (t_shell*)ft_calloc(1, sizeof(t_shell));
    if (!shell) {
        perror("Failed to initialize shell");
        exit(EXIT_FAILURE);
    }

    // Initialize the memory tracker within the shell
    shell->mem_tracker.head = NULL;

    // Initialize environment variables from envp, signal handlers, etc.
    // shell->env_vars = copy_env_vars(envp); // Example initialization, you'll need to implement copy_env_vars or similar
    setup_signals(shell); // Example function to setup custom signal handlers
    (void)envp;

    return shell;
}

int main(int argc, char **argv, char **envp)
{
    char* input;
    TokenNode* head = NULL;
    char **arg_input;
    int i;

    // Initialize your shell and get the shell structure
    t_shell* shell = initialize_shell(envp);
    (void)envp;
    if (argc == 3 && ft_strcmp(argv[1], "-c") == 0 && argv[2]) {
        arg_input = ft_split(argv[2], ';');
        if (!arg_input)
            shexit(shell, 1); // Assuming shexit now also takes a shell pointer for cleanup

        i = 0;
        while (arg_input[i])
        {
            ft_printf("non interactive mode\n");
            // Parse and execute arg_input[i], pass shell as needed
            i++;
        }
    }
    else
    {
        ft_printf("interactive mode\n");
        while (1)
        {
            input = readline("./minishell>$ ");
            if (input == NULL) {
                ft_printf("exit\n");
                break ;
            }
            if (ft_strlen(input) > 0)
            {
                add_history(input);

                // Adjust the following functions to accept `shell` as a parameter and use it internally
                create_tokens(shell, input, ' ', &head);
                CommandTable* command_table = create_command_table(shell, head);
                execute_command_table(shell, command_table);

                freeTokenList(head);
                head = NULL;
                free(input);
                // Assuming these functions are adjusted to use shell's memory management
            } else
                free(input); // Consider adjusting to use shell's memory management
        }
    }
    // Cleanup before exiting
    // shell_cleanup(shell); // Free all memory tracked by shell's MemTracker
    // free(shell); // Free the shell structure itself
    shexit(shell, 0); // Adjust shexit if necessary
    return 0;
}

// main 2
// int main(int argc, char **argv) {
//     if (isatty(STDIN_FILENO)) {
//         if (argc > 1) {
//             // Arguments are present; assume command execution mode
//             printf("Executing command...\n");
//             // Execute the command
//         } else {
//             // Interactive mode
//             printf("Interactive mode: awaiting commands.\n");
//             // Enter interactive loop
//         }
//     } else {
//         // Input is not from a terminal; non-interactive mode
//         printf("Non-interactive mode: processing input.\n");
//         // Process input (from file redirection or pipe)
//     }

//     if (!isatty(STDOUT_FILENO)) {
//         printf("Note: Output is redirected.\n");
//         // Adjust output handling as needed
//     }

//     return 0;
// }