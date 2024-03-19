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

// void sigint_handler(int sig_num)
// {
// 	(void)sig_num;
//     // Interrupt readline, and make it return NULL
//     // Note: You might need to use rl_replace_line("", 0); rl_done = 1; if you want to clear the current input
//     signal(SIGINT, sigint_handler); // Re-register handler to handle next SIGINT
//     ft_printf("\n$ "); // Display a new prompt on a new line
//     // fflush(stdout); // Make sure it's displayed immediately


//     // change the display if necessary
// }

// void sigquit_handler(int sig_num)
// {
// 	(void)sig_num;
//     // Do nothing handler for SIGQUIT
//     signal(SIGQUIT, sigquit_handler); // Re-register handler to handle next SIGQUIT
// }

// // signal handler
// void setup_signals(t_shell *shell)
// 	{
//     // Signal handling setup
//     // Set up signal handlers
//     signal(SIGINT, sigint_handler);  // Handle Ctrl-C
//     signal(SIGQUIT, sigquit_handler); // Handle Ctrl-\, do nothing
//     // Any other initialization
//     (void)shell;
// }

void sigint_handler(int sig_num)
{
    g_signal_received = sig_num;
    rl_replace_line("", 0);
    // Make readline ready to receive a new input
    // rl_done = 1;
    // ft_printf("\n$ "); // Display a new prompt
    write(STDOUT_FILENO, "\n$ ", 3);
    // fflush(stdout);
    // Re-registering signal handlers is not recommended here; see below for sigaction usage.
}

void sigquit_handler(int sig_num)
{
    g_signal_received = sig_num;
    // For SIGQUIT, you might not need to do anything special here.
}

void setup_signals(t_shell *shell)
{
    struct sigaction sa_int, sa_quit;

    // Set up SIGINT handler
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    // Set up SIGQUIT handler
    sa_quit.sa_handler = sigquit_handler;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);

    (void)shell;
}
