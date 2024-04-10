/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_operations_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:02:17 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 14:24:34 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_env_var	*find_return_env_var(t_env_var *head, const char *key)
{
	while (head != NULL)
	{
		if (ft_strcmp(head->key, key) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}
