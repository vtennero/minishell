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

int	is_valid_var_name(const char *str, int n)
{
	int	found_letter;
	int	i;
	int	len;

	if (n != -1)
		len = n;
	else
		len = (int)ft_strlen(str);
	i = 0;
	found_letter = 0;
	while (i < len)
	{
		if (str[i] == '\0')
			break ;
		if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]))
			return (0);
		if (ft_isalpha(str[i]))
			found_letter = 1;
		i++;
	}
	return (found_letter);
}

int	is_alloc_str(const char *str)
{
	const char	*equals_pos;

	equals_pos = ft_strchr(str, '=');
	return (equals_pos != NULL && *(equals_pos + 1) != '\0');
}

int	handle_incompl_alloc_str(t_shell *shell, const char *key, int nchar)
{
	char	*tmp;

	tmp = shell_strndup(shell, key, nchar);
	if (check_duplicates(shell, key, nchar))
		update_var(shell, tmp, "");
	else
		add_new_var(shell, tmp, NULL);
	return (1);
}

int	handle_decl_str(t_shell *shell, const char *key, int nchar)
{
	if (!check_duplicates(shell, key, nchar))
		decl_new_var(shell, key);
	return (1);
}

int	handle_alloc_str(t_shell *shell, const char *key, int nchar)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = shell_strndup(shell, key, nchar);
	tmp2 = shell_strdup(shell, key + nchar + 1);
	if (check_duplicates(shell, key, nchar))
		update_var(shell, tmp1, tmp2);
	else
		add_new_var(shell, tmp1, tmp2);
	return (1);
}

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

int	check_duplicates(t_shell *shell, const char *key, int nchar)
{
	t_env_var	*current;

	current = shell->env_head;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, key, nchar) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

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
	while (*tracer && strcmp((*tracer)->key, key) < 0)
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
	new_var->key = strdup(key);
	new_var->value = "";
	new_var->next = NULL;
	tracer = &shell->env_head;
	while (*tracer && strcmp((*tracer)->key, key) < 0)
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
