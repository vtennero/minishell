/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 16:38:52 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 10:45:40 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isNotEmpty(const char *str)
{
	// Check if string is NULL
	if (str == NULL)
		return (0); // String is NULL
	// Check if string is empty or consists of only whitespace characters
	for (size_t i = 0; str[i] != '\0'; i++)
	{
		if (!ft_isspace((unsigned char)str[i]))
			return (1);
		// String is not empty and contains non-whitespace characters
	}
	return (0); // String is empty or consists of only whitespace characters
}

int	end_with_pipe(const char *str)
{
	int	i;

	if (str == NULL)
		return (0); // String is NULL
	i = 0;
	while (str[i] != '\0')
		i++; // Move to the end of the string
	// Move backwards, skipping whitespace characters
	while (i > 0 && ft_isspace((unsigned char)str[i - 1]))
		i--;
	// Check if the last non-space character is a pipe symbol
	if (i > 0 && str[i - 1] == '|')
		return (1); // Last non-space character is a pipe symbol
	else
		return (0); // Last non-space character is not a pipe symbol
}
