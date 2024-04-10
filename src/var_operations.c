/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:20:29 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/31 21:28:37 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_new_var(t_shell *shell, const char *key, const char *value)
{
	t_env_var	*new_var;
	t_env_var	**tracer;

	new_var = shell_malloc(shell, sizeof(t_env_var));
	new_var->key = shell_strdup(shell, key);
	if (value)
		new_var->value = shell_strdup(shell, value);
	else
		new_var->value = shell_strdup(shell, "\0");
	new_var->next = NULL;
	tracer = &shell->env_head;
	while (*tracer && ft_strcmp((*tracer)->key, key) < 0)
	{
		tracer = &(*tracer)->next;
	}
	new_var->next = *tracer;
	*tracer = new_var;
}

void	decl_new_var(t_shell *shell, const char *key)
{
	t_env_var	*new_var;
	t_env_var	**tracer;

	new_var = shell_malloc(shell, sizeof(t_env_var));
	new_var->key = shell_strdup(shell, key);
	new_var->value = "";
	new_var->next = NULL;
	tracer = &shell->env_head;
	while (*tracer && ft_strcmp((*tracer)->key, key) < 0)
	{
		tracer = &(*tracer)->next;
	}
	new_var->next = *tracer;
	*tracer = new_var;
}

void	remove_var(t_shell *shell, const char *key, int nchar)
{
	t_env_var	**tracer;
	t_env_var	*to_delete;

	tracer = &shell->env_head;
	while (*tracer)
	{
		if (ft_strncmp((*tracer)->key, key, nchar) == 0)
		{
			to_delete = *tracer;
			*tracer = (*tracer)->next;
			(void)to_delete;
			return ;
		}
		tracer = &(*tracer)->next;
	}
}

void	update_var(t_shell *shell, const char *key, const char *value)
{
	t_env_var	*current;

	current = shell->env_head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->value = NULL;
			if (value)
				current->value = shell_strdup(shell, value);
			return ;
		}
		current = current->next;
	}
}
