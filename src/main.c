/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:45:31 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 15:36:36 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	shell = initialize_shell(envp);
	if (argc == 1)
		shell->is_interactive = 1;
	if (shell->is_interactive && argc == 1)
		interactive_mode(shell);
	else if (argc > 1)
		w_arg_mode(shell, argc, argv);
	else
		std_input_mode(STDIN_FILENO, shell);
	shexit(shell, 10);
	return (0);
}
