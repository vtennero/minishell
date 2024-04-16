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

void	rep_var(t_shell *shell, const char **in, char **out, t_env_var *env_v)
{
	const char	*var_start;
	int			var_name_len;
	char		*var_name;
	t_env_var	*var;

	var_start = ++(*in);
	while (ft_isalnum((unsigned char)**in) || **in == '_')
		(*in)++;
	var_name_len = *in - var_start;
	var_name = shell_strndup(shell, var_start, var_name_len);
	var = find_return_env_var(env_v, var_name);
	if (var)
	{
		ft_strcpy(*out, var->value);
		*out += ft_strlen(var->value);
	}
}

void	p_dol(t_shell *shell, const char **in, char **out, t_env_var *env_v)
{
	if (*(*in + 1) == '?')
		handle_exit_code(shell, in, out);
	else if (*(*in + 1) == '$' || *(*in + 1) == '\"')
	{
		*(*out)++ = *(*in)++;
		while (**in == '$')
			*(*out)++ = *(*in)++;
	}
	else if (*(*in + 1) == '\0' || isspace_not_eol(*(*in + 1)) == 1)
		*(*out)++ = *(*in)++;
	else
		rep_var(shell, in, out, env_v);
}

size_t	parse_var(t_shell *shell, const char *in, char *out, t_env_var *env_v)
{
	const char	*start_in;

	start_in = in;
	while (*in)
	{
		if (*in == '$')
			p_dol(shell, &in, &out, env_v);
		else if (*in == '\"' || isspace_not_eol(*in) == 1)
			break ;
		else
			*out++ = *in++;
	}
	*out = '\0';
	return (in - start_in);
}

char	*expand_var_one(t_shell *shell, const char *input)
{
	int		final_len;
	char	*expanded;
	size_t	adv_pos;

	final_len = calc_expanded_len(shell, input, shell->env_head);
	expanded = (char *)shell_malloc(shell, final_len);
	if (!expanded)
	{
		ft_printf("Memory allocation failed\n");
		return (NULL);
	}
	adv_pos = parse_var(shell, input, expanded, shell->env_head);
	(void)adv_pos;
	return (expanded);
}

char	*expand_var_two(t_shell *shell, const char *input, size_t *adv_pos)
{
	int		final_len;
	char	*expanded;
	size_t	position;

	*adv_pos = 0;
	final_len = calc_expanded_len(shell, input, shell->env_head);
	expanded = (char *)shell_malloc(shell, final_len);
	if (!expanded)
	{
		ft_printf("Memory allocation failed\n");
		return (NULL);
	}
	position = parse_var(shell, input, expanded, shell->env_head);
	if (adv_pos != NULL)
		*adv_pos = position;
	return (expanded);
}
