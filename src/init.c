/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:46:06 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/20 21:02:47 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_key_value(const char *str, char **key, char **value)
{
	const char	*equals_pos;
	int			key_len;

	equals_pos = ft_strchr(str, '=');
	if (equals_pos != NULL)
	{
		key_len = equals_pos - str;
		*key = (char *)malloc(key_len + 1);
		ft_strncpy(*key, (char *)str, key_len);
		(*key)[key_len] = '\0';
		*value = ft_strdup(equals_pos + 1);
		return (1);
	}
	return (0);
}

void	insert_sorted_env_var(t_shell *shell, char *key, char *value)
{
	t_env_var	*new_var;
	t_env_var	*current;

	new_var = malloc(sizeof(t_env_var));
	new_var->key = key;
	new_var->value = value;
	new_var->next = NULL;
	if (shell->env_head == NULL || ft_strcmp(key, shell->env_head->key) < 0)
	{
		new_var->next = shell->env_head;
		shell->env_head = new_var;
	}
	else
	{
		current = shell->env_head;
		while (current->next != NULL && ft_strcmp(current->next->key, key) < 0)
			current = current->next;
		new_var->next = current->next;
		current->next = new_var;
	}
}

void	create_env_var_list(t_shell *shell, char **envp)
{
	char	*key;
	char	*value;
	int		i;

	i = 0;
	shell->env_head = NULL;
	while (envp[i] != NULL)
	{
		if (split_key_value(envp[i], &key, &value))
			insert_sorted_env_var(shell, key, value);
		i++;
	}
}

t_shell	*initialize_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
	if (!shell)
	{
		perror("Failed to initialize shell");
		exit(EXIT_FAILURE);
	}
	shell->mem_tracker.head = NULL;
	shell->token_head = NULL;
	shell->last_exit_status = 0;
	shell->is_interactive = 0;
	shell->envp = envp;
	if (isatty(STDIN_FILENO))
	{
		if (isatty(STDOUT_FILENO))
			shell->is_interactive = 1;
	}
	setup_signals(shell);
	create_env_var_list(shell, envp);
	return (shell);
}
