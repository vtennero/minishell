/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_operations_handlers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:05:07 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 14:05:14 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
