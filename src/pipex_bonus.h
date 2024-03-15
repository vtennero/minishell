/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cliew <cliew@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 17:01:53 by cliew             #+#    #+#             */
/*   Updated: 2024/01/31 19:31:06 by cliew            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
// Existing libft
# include "./libft_max/getnextline/gnl.h"
# include "./libft_max/libft/libft.h"
# include "./libft_max/printf/printf.h"

// Standard
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

// Pipex
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <unistd.h>
// Error Handiling
# define ERR_INVALID_PARAMS "pipex: invalid parameters"
# define ERR_INVALID_FILE "pipex: No such file or directory"
# define ERR_INVALID_CMD "\n pipex: command not found \n"
# define STDIN_FILENO 0
# define STDOUT_FILENO 1

// Struc

typedef struct s_params
{
	int		fin;
	int		fout;
	int		argc;
	char	**argv;
	char	**envp;
	int		pipefd[2];
}			t_in;

typedef struct Command {
    char* name;
    char** args;
    int arg_count;
	int fin; // things that redirect in, stdin for the cmd fd, if its -99, means it takes in from previous cmd
	int fout; // things that this function redicrect out, stdout for the cmd,if its -99, means it outputs to next cmd
    struct Command* next;
} Command;

char		*check_path(char **envp);
char		*locate_cmd(char **paths, char *cmd);
char		*find_env_path(char **envp);
void		free_array(char **v);
char		**find_cmd_paths(char **envp);
char		**ft_split_cmd_args(char *s);
int			run_cmd(char *cmd, char **envp);
int			pipex(t_in in,Command cmd);
#endif
