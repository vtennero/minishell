/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 11:04:01 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/20 11:04:01 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig_num)
{
	g_signal_received = sig_num;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_signal_received = 0;
}

void	setup_signals(t_shell *shell)
{
	struct sigaction	sa_int;
	struct sigaction	sa_ignore;

	if (shell->nested_shell == 0)
	{
		sa_int.sa_handler = sigint_handler;
		sigemptyset(&sa_int.sa_mask);
		sa_int.sa_flags = 0;
		sigaction(SIGINT, &sa_int, NULL);
	}
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGQUIT, &sa_ignore, NULL);
	(void)shell;
}
