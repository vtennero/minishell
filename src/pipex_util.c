/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 17:08:33 by cliew             #+#    #+#             */
/*   Updated: 2024/04/02 16:06:50 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_path(char **envp)

{
	while (*envp++ != NULL)
	{
		if (ft_strnstr(*envp, "PATH=", 5) != NULL)
		{
			return (*(envp));
		}
	}
	return (NULL);
}

char	*locate_cmd(char **paths, char *cmd)
{
	char	*_cmd;
	char	*cmd_path;

	if (access(cmd, X_OK) >= 0)
		return (ft_strdup(cmd));
	_cmd = ft_strjoin("/", cmd);
	cmd_path = NULL;
	while (paths && *paths)
	{
		cmd_path = ft_strjoin(*paths, _cmd);
		if (access(cmd_path, X_OK) >= 0)
		{
			// free(cmd_path);
			free(_cmd);
			return (cmd_path);
			// break ;
		}
		free(cmd_path);
		cmd_path = NULL;
		paths++;
	}
	free(_cmd);
	return (cmd_path);
}

char	*find_env_path(char **envp)
{
	while (envp && *envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
		{
			return (ft_strdup(&(*envp)[5]));
		}
		envp++;
	}
	return (NULL);
}

void	free_array(char **v)
{
	char	**p;

	if (!v)
		return ;
	p = v;
	while (*v)
	{
		free(*v);
		v++;
	}
	free(p);
}

char	**find_cmd_paths(char **envp)
{
	char	**paths;
	char	*path;

	path = find_env_path(envp);
	paths = ft_split(path, ':');
	if (path)
		free(path);
	return (paths);
}


int	ft_strchr_count(const char *s, int c)
{
	int	count;

	if (s == NULL)
		return (0);
	count = 0;
	while (*s)
	{
		if (*s == c)
			count++;
		s++;
	}
	return (count);
}
char	*ft_strjoin_nconst(char *s1, char *s2)
{
	char	*res;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (0);
	i = 0;
	while (s1[i])
		i++;
	j = 0;
	while (s2[j])
		j++;
	res = ft_calloc(i + j + 1, 1);
	if (!res)
		return (NULL);
	i = -1;
	j = -1;
	while (s1[++i])
		res[i] = s1[i];
	while (s2[++j])
		res[i + j] = s2[j];
	return (res);
}
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
