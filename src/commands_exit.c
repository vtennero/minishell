/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:11:34 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/23 22:20:19 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	adjust_exit_code(int n)
{
	while (n > 255)
		n -= 256;
	while (n < 0)
		n += 256;
	return (n);
}

int	is_valid_number(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str != '+' && *str != '-' && !ft_isdigit(*str))
		return (0);
	str++;
	while (*str != '\0')
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	builtin_exit(t_shell *shell, char **args, int n_args)
{
	if (n_args >= 2)
	{
		perror("too many arguments ");
		shell->last_exit_status = 1;
	}
	else if (n_args == 1)
	{
		if (is_valid_number(args[0]) == 0)
			shell->last_exit_status = 2;
		else
			shell->last_exit_status = adjust_exit_code(ft_atoi(args[0]));
	}
	ft_printf("exit\n");
	shexit(shell, 0);
	// return (0); // whatever the exit code is
}
