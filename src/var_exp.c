/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_exp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:56:22 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/31 22:37:53 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_var *findEnvVar(t_env_var *head, const char *key) {
	while (head != NULL) {
		if (ft_strcmp(head->key, key) == 0) {
			return head;
		}
		head = head->next;
	}
	return NULL;
}

int getEnvVarLength(t_shell *shell, const char **input, t_env_var *envVars)
{
	const char  *varStart;
	int         varNameLength;
	char        *varName;
	t_env_var	*var;

	varNameLength = 0;
	varStart = *input + 1;
	while (ft_isalnum((unsigned char)**input) || **input == '_')
		(*input)++;
	varNameLength = *input - varStart;
	varName = shell_strndup(shell, varStart, varNameLength);
	var = findEnvVar(envVars, varName);
	if (var)
		return ft_strlen(var->value);
	else
		return (0);
}

void incrementLengthAndInput(const char** input, int* length, int a, int b)
{
    *input += a;
    *length += b;
}

int calculateExpandedLength(t_shell *shell, const char *input, t_env_var *envVars)
{
	int	length;

	length = 0;
	while (*input)
	{
		if (*input == '$')
		{
			if (*(input + 1) == ' ')
			    incrementLengthAndInput(&input, &length, 2, 2);
			else if (*(input + 1) == '?')
			    incrementLengthAndInput(&input, &length, 2, calc_int_len(shell->last_exit_status));
			else if (*(input + 1) == '$')
			{
				while (*input == '$')
			    	incrementLengthAndInput(&input, &length, 1, 1);
			}
			else
				incrementLengthAndInput(&input, &length, 1, getEnvVarLength(shell, &input, envVars));
		}
		else
			incrementLengthAndInput(&input, &length, 1, 1);
	}
	return (length + 1);
}

void replaceEnvVar(t_shell *shell, const char **input, char **output, t_env_var *envVars)
{
	const char	*varStart;
	int			varNameLength;
	char		*varName;
	t_env_var	*var;

	varStart = ++(*input);
	while (ft_isalnum((unsigned char)**input) || **input == '_')
		(*input)++;
	varNameLength = *input - varStart;
	varName = shell_strndup(shell, varStart, varNameLength);
	var = findEnvVar(envVars, varName);
	if (var)
	{
		ft_strcpy(*output, var->value);
		*output += ft_strlen(var->value);
	}
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

size_t replaceVariables(t_shell *shell, const char *input, char *output, t_env_var *envVars)
{
	const char	*startInput;
	
	startInput = input;
	while (*input)
	{
		if (*input == '$')
		{
			if (*(input + 1) == ' ')
				break;
			else if (*(input + 1) == '?' )
			{
				cpy_exit_code(output, shell->last_exit_status);
				output += calc_int_len(shell->last_exit_status);
				input += 2;
			}
			else if (*(input + 1) == '$' || *(input + 1) == '\"')
			{
				*output++ = *input++;
				if (*(input) == '$')
				{
					while (*input == '$')
						*output++ = *input++;
				}
			}
			else if (*(input + 1) == '\0')
				*output++ = *input++;
			else
				replaceEnvVar(shell, &input, &output, envVars);
		}
		else if (*input == '\"' || *input == '\'' || *input == ' ')
			break ;
		else {
			*output++ = *input++;
		}
	}
	*output = '\0';
	return input - startInput;

}

char	*expandVariables(t_shell *shell, const char *input)
{
	int		finalLength;
	char	*expanded;
	size_t	advancedPosition;

	finalLength = calculateExpandedLength(shell, input, shell->env_head);
	expanded = (char *)shell_malloc(shell, finalLength);
	if (!expanded) {
		printf("Memory allocation failed\n");
		return NULL;
	}
	advancedPosition = replaceVariables(shell, input, expanded, shell->env_head);
	(void)advancedPosition;
	return (expanded);
}

char	*expandVariables2(t_shell *shell, const char *input, size_t *advancedPosition)
{
	int		finalLength;
	char	*expanded;
	size_t	position;

	finalLength = calculateExpandedLength(shell, input, shell->env_head);
	expanded = (char *)shell_malloc(shell, finalLength);
	if (!expanded) {
		printf("Memory allocation failed\n");
		return NULL;
	}
	position = replaceVariables(shell, input, expanded, shell->env_head);
	if (advancedPosition != NULL)
		*advancedPosition = position;
	return (expanded);
}
