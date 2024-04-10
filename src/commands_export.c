/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:11:37 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/31 17:25:35 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_env_arg(t_shell *shell, const char *arg)
{
	int	nchar;

	nchar = find_index_char(arg, '=');
	if (is_valid_var_name(arg, nchar) == 0)
	{
		shell->last_exit_status = 1;
		ft_putstr_fd(" not a valid identifier", 2);
		return (1);
	}
	else if (nchar == (int)ft_strlen(arg) - 1)
	{
		handle_incompl_alloc_str(shell, arg, nchar);
		return (0);
	}
	else if (nchar == -1)
	{
		handle_decl_str(shell, arg, nchar);
		return (0);
	}
	else if (is_alloc_str(arg))
	{
		handle_alloc_str(shell, arg, nchar);
		return (0);
	}
	return (0);
}

void	print_export(t_shell *shell, int is_export)
{
	t_env_var	*current;

	current = shell->env_head;
	while (current)
	{
		if (shell->is_interactive == 1 && is_export == 1)
			ft_printf("declare -x ");
		ft_printf("%s", current->key);
		if (current->value != NULL)
		{
			if (is_export == 1)
				ft_printf("=\"%s\"", current->value);
			else
				ft_printf("=%s", current->value);
		}
		ft_printf("\n");
		current = current->next;
	}
}

int	builtin_export(t_shell *shell, char **args, int n_args)
{
	int	i;
	int	error;

	error = 0;
	i = 0;
	if (n_args == 0)
		print_export(shell, 1);
	else
	{
		while (i < n_args)
		{
			error = process_env_arg(shell, args[i]);
			if (error == 1)
				return (1);
			i++;
		}
	}
	return (0);
}
