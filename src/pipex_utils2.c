/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:24:55 by cliew             #+#    #+#             */
/*   Updated: 2024/04/16 07:28:38 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup_ignore(const char *s, char ignore)
{
	char	*dup;
	size_t	slen;
	size_t	i;

	slen = ft_strlen(s);
	i = ft_strchr_count(s, ignore);
	dup = (char *)malloc((slen - i + 1) * sizeof(char));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != ignore)
		{
			dup[i] = *s;
			i++;
		}
		s++;
	}
	dup[i] = 0;
	return (dup);
}

int	ft_puterr(char *s, int ret)
{
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
	exit (ret);
}

int	find_env_var(t_env_var *list, const char *key)
{
	while (list != NULL)
	{
		if (ft_strcmp(list->key, (char *)key) == 0)
			return (1);
		list = list->next;
	}
	return (0);
}

void set_fd(t_cmd *cmd)
{
	int	fd;

	if (cmd->redirect_in != NULL)
		fd = open(cmd->redirect_in, O_RDWR);
	if (cmd->fin != -1 && cmd->fin!=0 && cmd->fin != -99)
		cmd->fin = fd;
	if (cmd->fin==-1)
		return;
	if (cmd->fout!=-1 && cmd->fout!=0 && cmd->fout!= -99) 
	{
		if (cmd->redirect_app != NULL)
			fd = open(cmd->redirect_app, O_RDWR | O_CREAT | O_APPEND, 0666);
		else if (cmd->redirect_out != NULL)
			fd = open(cmd->redirect_out, O_RDWR | O_CREAT | O_TRUNC, 0666);
		cmd->fout = fd;
	}
	// print_command(cmd);
}
