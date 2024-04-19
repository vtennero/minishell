/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_exp_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:22:35 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/19 11:48:25 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_code(t_shell *shell, const char **input, char **output)
{
	cpy_exit_code(*output, shell->last_exit_status);
	*output += calc_int_len(shell->last_exit_status);
	*input += 2;
}

void	inc(const char **input, int *length, int a, int b)
{
	*input += a;
	*length += b;
}

char	*cpy_exit_code(char *str, int n)
{
	int		i;

	i = calc_int_len((n));
	str[i] = '\0';
	if (str)
	{
		while (i > 0)
		{
			str[--i] = ft_abs(n % 10) + '0';
			n /= 10;
		}
	}
	return (str);
}

int	get_env_var_len(t_shell *shell, const char **input, t_env_var *env_vars)
{
	const char	*var_start;
	int			var_name_len;
	char		*var_name;
	t_env_var	*var;

	(*input)++;
	var_name_len = 0;
	var_start = *input;
	while (ft_isalnum((unsigned char)**input) || **input == '_')
		(*input)++;
	var_name_len = *input - var_start;
	var_name = shell_strndup(shell, var_start, var_name_len);
	var = find_return_env_var(env_vars, var_name);
	if (var)
		return (ft_strlen(var->value));
	else
		return (0);
}

int	calc_expanded_len(t_shell *shell, const char *input, t_env_var *env_vars)
{
	int	len;

	len = 0;
	while (*input)
	{
		if (*input == '$')
		{
			if (*(input + 1) == ' ')
				inc(&input, &len, 2, 2);
			else if (*(input + 1) == '?')
				inc(&input, &len, 2, calc_int_len(shell->last_exit_status));
			else if (*(input + 1) == '$')
			{
				while (*input == '$')
					inc(&input, &len, 1, 1);
			}
			else
				inc(&input, &len, 0, get_env_var_len(shell, &input, env_vars));
		}
		else
			inc(&input, &len, 1, 1);
	}
	return (len + 1);
}
