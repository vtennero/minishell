/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 11:27:24 by toto              #+#    #+#             */
/*   Updated: 2024/02/22 15:07:30 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void sigint_handler(int sig_num)
{
	(void)sig_num;
    // Interrupt readline, and make it return NULL
    // Note: You might need to use rl_replace_line("", 0); rl_done = 1; if you want to clear the current input
    signal(SIGINT, sigint_handler); // Re-register handler to handle next SIGINT
    ft_printf("\n$ "); // Display a new prompt on a new line
    // fflush(stdout); // Make sure it's displayed immediately


    // change the display if necessary
}

void sigquit_handler(int sig_num)
{
	(void)sig_num;
    // Do nothing handler for SIGQUIT
    signal(SIGQUIT, sigquit_handler); // Re-register handler to handle next SIGQUIT
}

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
