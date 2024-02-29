/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:43:36 by toto              #+#    #+#             */
/*   Updated: 2024/02/19 15:36:55 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// needs to be updated with shell structure as parameter

// void user_input_subfunction(TokenNode** head, TokenNode* last_pipe)
// {
//     char* user_input = NULL;
//     size_t bufsize = 0;
// 	(void)head; // Prevent unused parameter warning

//     // Display a secondary prompt and wait for user input
//     printf("> ");
//     getline(&user_input, &bufsize, stdin);
    
//     // Tokenize the user input and append to the list
//     // Assume ft_split_to_list is adapted to append to existing list and identify token types
//     ft_split_to_list(user_input, ' ', &last_pipe->next);
    
//     // Free the acquired line buffer
//     free(user_input);
// }


// void process_pipes(TokenNode** head)
// {
//     TokenNode* current = *head;
//     // TokenNode* previous = NULL;

//     while (current != NULL) {
//         if (current->token.type == TOKEN_PIPE) {
//             if (current->next == NULL) {
//                 // Case: Pipe is the last element; prompt user for input
//                 user_input_subfunction(head, current);
//                 break; // After enriching the list, break out of the loop
//             } else if (current->next->token.type == TOKEN_PIPE) {
//                 // Case: Two pipes in succession; error
//                 fprintf(stderr, "Error: Misplaced pipe\n");
//                 break;
//             } else {
//                 // Case: Next token should be a command
//                 current->next->token.type = TOKEN_COMMAND;
//             }
//         }
//         // previous = current;
//         current = current->next;
//     }
// }
