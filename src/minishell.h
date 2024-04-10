/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:57:25 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/10 10:57:25 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/wait.h>
# include <termios.h>
# include <sys/stat.h>

# define ERR_INVALID_CMD " : command not found"
# define STDIN_FILENO 0
# define STDOUT_FILENO 1

extern volatile sig_atomic_t	g_signal_received;
/*
** ================== STRUCTURES ==================
*/
/*
** -- MEMORY --
*/
typedef struct s_memnode
{
	void				*ptr;
	struct s_memnode	*next;
}	t_memnode;

typedef struct s_memtracker
{
	t_memnode	*head;
}	t_memtracker;
/*
** -- PARSING --
*/
typedef struct s_token
{
	char	*value;
	int		type;
}	t_token;

typedef struct s_token_node
{
	t_token				token;
	struct s_token_node	*next;
}	t_token_node;

enum e_token_type
{
	TOKEN_COMMAND,
	TOKEN_ARG,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_PIPE,
	TOKEN_ENV_VAR,
	TOKEN_EXIT_STATUS,
	TOKEN_D_Q,
	TOKEN_S_Q,
	TOKEN_S_DOL,
	TOKEN_EOL,
	TOKEN_INV_COMMAND
};

/*
** -- COMMANDS --
*/
typedef struct s_cmd
{
	char			*name;
	char			**args;
	char			**execv_args;
	char			*cmd_path;
	int				arg_count;
	char			*redirect_in;
	char			*redirect_out;
	char			*redirect_append;
	char			*heredoc_delimiter;
	char			*heredoc_temp_path;
	int				fd_in;
	int				fd_out;
	int				fin;
	int				fout;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_cmd_table
{
	t_cmd	*head;
	int		command_count;
}	t_cmd_table;
/*
** -- ENV --
*/
typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;
/*
** -- MAIN --
*/
typedef struct s_shell
{
	t_env_var			*env_head;
	int					last_exit_status;
	struct sigaction	*signals;
	int					is_interactive;
	char				**envp;
	int					pipefd[2];
	int					std_in;
	int					std_out;
	int					line_count;
	char				*readfile;
	pid_t				pid;
	t_memtracker		mem_tracker;
	t_token_node		*token_head;
	t_cmd_table			*table;
}	t_shell;
/*
** ================== INITIALIZATION ==================
*/
t_shell			*initialize_shell(char **envp);
void			create_env_var_list(t_shell *shell, char **envp);

/*
** -- MODES --
*/
void			interactive_mode(t_shell *shell);
void			w_arg_mode(t_shell *shell, int argc, char **argv);
void			std_input_mode(int fd, t_shell *shell);
int				not_empty(const char *str);
int				end_with_pipe(const char *str);
int				pipe_extension(t_shell *shell, char *input);

/**
** -- SIGNALS --
*/
void			sigint_handler(int sig_num);
void			setup_signals(t_shell *shell);

/*
** ================== PARSING ==================
*/
/*
** -- TOKENIZATION --
*/
void			create_tokens(t_shell *shell, const char *s);
void			add_token(t_shell *shell, const char *value, int type);
int				get_token_type(const char *token_text);
char			*parse_tokens(t_shell *shell, const char *s);
int				isspace_not_eol(int ch);
void			set_token_commands(t_shell *shell);
int				is_valid_cmd(t_shell *shell, char *cmd_name);
int				is_only_spaces(char *str);
int				is_special(const char *str);
int				is_redirect(int type, int *after_redirect);
/*
** -- QUOTES --
*/
char			*process_quoting(t_shell *shell, const char **s, char *result);
char			*process_single_quote(t_shell *shell, const char **s);
char			*process_double_quote(const char **s, t_shell *shell);
int				get_non_expanded_var_length(char *var);
/*
** -- VARIABLE EXPANSION --
*/
char			*expand_var_one(t_shell *shell, const char *input);
char			*expand_var_two(t_shell *shell, const char \
*input, size_t *adv_pos);
t_env_var		*find_return_env_var(t_env_var *head, const char *key);
void			handle_exit_code(t_shell *shell, \
const char **input, char **output);
int				calc_expanded_len(t_shell *shell, \
const char *input, t_env_var *env_vars);
char			*cpy_exit_code(char *str, int n);
/*
** -- HEREDOC --
*/
void			prepare_heredocs_in_command_table(t_cmd_table *table);
void			cleanup_heredocs_in_command_table(t_cmd_table *table);

void			cleanup_heredocs_in_command_table(t_cmd_table *table);
void			write_heredoc_to_file(int fd, const char *delimiter);
char			*handle_heredoc(const char *delimiter);
void			prepare_heredocs_in_command_table(t_cmd_table *table);
void			cleanup_temp_file(char *tempFilePath);
int				create_temp_file(char **tempFilePath);

/*
** ================== COMMANDS ==================
*/
/*
** -- COMMAND TABLE --
*/
t_cmd_table		*create_command_table(t_shell *shell, t_token_node *tokens);
void			execute_command_table(t_shell *shell, t_cmd_table *table);
void			free_command_table(t_cmd_table *table);
void			set_token_commands(t_shell *shell);

/*
** -- BUILT_IN COMMANDS --
*/
int				builtin_cd(t_shell *shell, char **args, int n_args);
int				builtin_pwd(void);
int				builtin_echo(t_shell *shell, \
char **args, int n_args, t_cmd *cmd);
int				builtin_unset(t_shell *shell, char **args, int n_args);
int				builtin_env(t_shell *shell);




t_cmd_table		*initialize_command_table(t_shell *shell);
void			set_rin(t_shell *shell, t_cmd *cmd, char *filename);
void			set_rout(t_shell *shell, t_cmd *cmd, char *filename, int append);
void			free_command_table(t_cmd_table *table);\

t_cmd_table		*create_command_table(t_shell *shell, t_token_node *tokens);
void			process_token_nodes(t_shell *shell, t_cmd_table *table,
				t_token_node *tokens);
void			update_token_node(t_token_node **ct);
t_cmd			*process_command_set(t_shell *shell, t_cmd_table *table,
				t_token_node **ct, int *pipe_exist);
void			update_head(t_cmd *current_command, t_cmd_table *table);
void			pipe_modify_fin_fout(t_token_node *current_token, t_cmd *cc,
				int *pipe_exist);
t_cmd			*create_command_entry(t_shell *shell, char *name);
void			add_argument(t_shell *shell, t_cmd *cmd, char *arg);
t_cmd			*create_command_set(t_shell *shell, t_token_node *node);
void			handle_token(t_shell *shell, t_token_node *ct, t_cmd *cc);


/*
** :: EXIT ::
*/
int				builtin_exit(t_shell *shell, char **args, int n_args);
int				is_valid_number(const char *str);
int				adjust_exit_code(int n);
/*
** :: EXPORT ::
*/
int				builtin_export(t_shell *shell, char **args, int n_args);
int				process_env_arg(t_shell *shell, const char *arg);
int				check_duplicates(t_shell *shell, const char *key, int nchar);
void			update_var(t_shell *shell, const char *key, const char *value);
void			add_new_var(t_shell *shell, const char *key, const char *value);
void			decl_new_var(t_shell *shell, const char *key);
void			remove_var(t_shell *shell, const char *key, int nchar);
void			print_export(t_shell *shell, int is_export);
int				handle_incompl_alloc_str(t_shell \
*shell, const char *key, int nchar);
int				handle_decl_str(t_shell *shell, const char *key, int nchar);
int				handle_alloc_str(t_shell *shell, const char *key, int nchar);
int				is_alloc_str(const char *str);
int				is_valid_var_name(const char *str, int n);
/*
** ================== PIPES ==================
*/


int	is_custom_cmd(char *name);
void	check_child_error(t_shell *shell, t_cmd *cmd, char *error);
int	execute_command_pipex(int prev_pipe, t_cmd *cmd, t_shell *shell, int parent);
void	last_pipe(t_shell *shell, t_cmd *cmd, int prev_pipe, int *status);
int	pipex(t_cmd *cmd, t_shell *shell);

//pipex2
void	free_cmd(t_cmd *cmd);
int	builtin_cmd(t_cmd *command, t_shell *shell);
void	run_execv(char **cmd_args, char *cmd_path, t_shell *shell);
int	custom_cmd(char **cmd_args, char *cmd_path, t_cmd *cmd, t_shell *shell);
void	prepend_linecount(t_shell *shell);


//pipex3
int	run_cmd(t_cmd *command, t_shell *shell);
void	pipe_heredoc(t_cmd *cmd, t_shell *shell);
void	check_finfout(int prev_pipe, t_cmd *cmd, t_shell *shell);
void	clean_fd(t_shell *shell, int std_in, int std_out, t_cmd *cmd);
void	handle_status_error(int status, t_cmd *cmd, t_shell *shell);

//pipex 4
int	assign_cmd_args(t_shell *shell, t_cmd *command, char **envp);
int	is_directory(const char *path);
int	check_error(t_cmd *cmd, t_shell *shell, int parent);
char	*locate_cmd(char **paths, char *cmd);
char	*check_path(char **envp);

//pipex util
char	*find_env_path(char **envp);
void	free_array(char **v);
char	**find_cmd_paths(char **envp);
int	ft_strchr_count(const char *s, int c);
char	*ft_strjoin_nconst(char *s1, char *s2);

//pipex util2
char	*ft_strdup_ignore(const char *s, char ignore);
int	ft_puterr(char *s, int ret);
int	find_env_var(t_env_var *list, const char *key);


// set token cmd
int				check_if_valid_cmd(t_token_node *node);
void	set_commands_check(t_shell *shell, t_token_node *node,
		int *after_redirect, int *pipe_exist);
/*
** ================== MEMORY ==================
*/
void			shexit(t_shell *shell, int exit_code);
void			*shell_malloc(t_shell *shell, size_t size);
void			shell_free(t_shell *shell, void *ptr);
void			shell_cleanup(t_shell *shell);
char			*shell_strdup(t_shell *shell, const char *s);
char			*shell_strndup(t_shell *shell, const char *s, size_t n);
char			*shell_strjoin(t_shell *shell, char const *s1, char const *s2);
char			*shell_itoa(t_shell *shell, int n);
/*
** ================== UTILS ==================
*/
const char		*skip_delimiters(const char *s, char c);
int				find_index_char(const char *str, char c);
int				calc_int_len(int num);
int				not_empty(const char *str);
int				is_directory(const char *path);

/*
** -- DEBUG --
*/
void			print_tokens(t_token_node *head);
void			print_command_table(const t_cmd_table *table);
int				is_token_type_present(t_token_node *head, int type);
void			print_command(const t_cmd *cmd);

#endif
