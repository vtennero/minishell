/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:30:17 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/15 15:32:10 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*export_exit_code(t_shell *shell)
{
	char	*exit_code;

	exit_code = ft_itoa(shell->last_exit_status);
	return exit_code;
}

int adjust_exit_code(int n)
{
	// If n is higher than 255, subtract 256 until it's not
	while (n > 255) {
		n -= 256;
	}

	// If n is lower than 0, add 256 until it's between 0 and 255
	while (n < 0) {
		n += 256;
	}

	return n;
}

int is_valid_number(const char *str)
{
	// Check if the string is empty
	if (!str || *str == '\0') {
		return 0; // Not a valid number
	}

	// Check if the first character is +, -, or a digit
	if (*str != '+' && *str != '-' && !ft_isdigit(*str)) {
		return 0; // Not a valid number
	}

	// Move to the next character
	str++;

	// Check that every other character is a digit
	while (*str != '\0') {
		if (!ft_isdigit(*str)) {
			return 0; // Not a valid number
		}
		str++;
	}

	return 1; // Valid number
}