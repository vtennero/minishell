/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:50:11 by toto              #+#    #+#             */
/*   Updated: 2024/02/15 11:51:54 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void initialize_shell()
	{
    // Signal handling setup
    // Set up signal handlers
    signal(SIGINT, sigint_handler);  // Handle Ctrl-C
    signal(SIGQUIT, sigquit_handler); // Handle Ctrl-\, do nothing
    // Any other initialization
}


// int main(void) {
//     char* input;
//     TokenNode* head = NULL; // Head of the list, initially NULL

//     // Initialize your shell (environment variables, signal handling, etc.)
//     initialize_shell();

//     while (1) 
//     {
//         input = readline("$ "); // Display prompt and read input with readline
//         if (input == NULL) {
//             ft_printf("\nExiting...\n"); // Handle EOF (Ctrl+D)
//             break;
//         }
//         if (ft_strlen(input) > 0)
//         {
//             add_history(input); // Only add non-empty commands to history

//             // Process the input
//             // (Tokenize, parse, and execute commands here)
//             ft_split_to_list(input, ' ', &head);
// 			// ft_printf("%s\n", input);
// 	        printTokens(head);

//             free(input); // Free the input after processing
//             freeTokenList(head);
//             head = NULL;
//         } else {
//             free(input); // Also free input if it's an empty line
//         }
//     }

//     // Cleanup before exiting, if necessary
//     // (e.g., free allocated resources, save history)

//     return EXIT_SUCCESS;
// }	


int main(void)
{
    char* input;
    TokenNode* head = NULL; // Assume you have this defined elsewhere

    // Initialize your shell (environment variables, signal handling, etc.)
    initialize_shell();



    while (1) {
        input = readline("$ "); // Display prompt and read input with readline
        if (input == NULL) {
            ft_printf("exit\n"); // Handle EOF (Ctrl+D)
            break;
        }
        if (strlen(input) > 0) {
            add_history(input); // Only add non-empty commands to history

            // Process the input
            // (Tokenize, parse, and execute commands here)
            ft_split_to_list(input, ' ', &head);
			// ft_printf("%s\n", input);
	        printTokens(head);

            free(input); // Free the input after processing
            freeTokenList(head);
            head = NULL;
        } else {
            free(input); // Also free input if it's an empty line
        }
    }

    // Cleanup before exiting, if necessary

    return EXIT_SUCCESS;
}