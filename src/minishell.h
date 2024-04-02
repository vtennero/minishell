/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/03/20 15:57:52 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/20 15:57:52 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
// # include <sys/types.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/wait.h>
# include <termios.h>

extern volatile sig_atomic_t g_signal_received;
/*
** ================== STRUCTURES ==================
*/
/*
** -- MEMORY --
*/
typedef struct MemNode
{
	void *ptr;            // Pointer to the allocated memory
	struct MemNode *next; // Next node in the list
} MemNode;

typedef struct
{
	MemNode *head; // Head of the list of allocated memory blocks
} MemTracker;
/*
** -- PARSING --
*/
typedef struct
{
	char *value; // The actual token, e.g., "ls", "-l", ">", etc.
	int type;    // Type of the token (command, argument, redirection, pipe,etc.
} Token;

typedef struct TokenNode
{
	Token token;
	struct TokenNode *next;
} TokenNode;

typedef enum
{
	TOKEN_COMMAND,       // 0
	TOKEN_ARG,           // 1
	TOKEN_REDIR_IN,      // <       2
	TOKEN_REDIR_OUT,     // >       3
	TOKEN_REDIR_APPEND,  // >>  4
	TOKEN_REDIR_HEREDOC, // << 5
	TOKEN_PIPE,          // |        6
	TOKEN_ENV_VAR,       // $VAR     7
	TOKEN_EXIT_STATUS,   // $?    8
	TOKEN_D_Q,           //              9
	TOKEN_S_Q,           //              10
	TOKEN_S_DOL,         //              11
	TOKEN_EOL, 
	TOKEN_INV_COMMAND          //              12
} TokenType;

typedef enum
{
	NO_QUOTES,
	SINGLE_QUOTES,
	DOUBLE_QUOTES
} QuoteType;
/*
** -- COMMANDS --
*/
typedef enum
{
	CMD_BUILTIN,
	CMD_EXTERNAL
} CommandType;

typedef struct Command
{
	CommandType type;
	char *name;
	char **args;
	char **execv_args;
	char	*cmd_path;

	int arg_count;
	char *redirect_in;
	char *redirect_out;
	char *redirect_append;
	// char *redirect_heredoc;
	char *heredoc_delimiter;
	char *heredoc_temp_path; 
	int fd_in;  // File descriptor for input redirection
	int fd_out; // File descriptor for output redirection
	int fin;    // File descriptor for input redirection
	int fout;   // File descriptor for output redirection
	struct Command *next;
} Command;


typedef struct
{
	Command *head;     // Head of the list of commands
	int command_count; // Number of commands in the table
} CommandTable;

typedef enum
{
	REDIRECT_NONE,   // No redirection
	REDIRECT_INPUT,  // Input redirection ('<')
	REDIRECT_OUTPUT, // Output redirection ('>')
	REDIRECT_APPEND  // Output append redirection ('>>')
} RedirectionType;
/*
** -- ENV --
*/
typedef struct s_env_var
{
	char *key;              // For storing the variable name
	char *value;            // For storing the variable value
	struct s_env_var *next; // Pointer to the next node in the list
} t_env_var;
/*
** -- MAIN --
*/
typedef struct s_shell
{
	t_env_var *env_head;      
		// Head of the linked list of environment variables for export
	int last_exit_status;      // Exit status of the last executed command
	struct sigaction *signals; // Custom signal handlers
	int is_interactive;
	char** envp;
	int pipefd[2];
	int std_in;
	int std_out;
	pid_t pid;
	MemTracker mem_tracker;
	TokenNode *token_head;
} t_shell;
/*
** ================== INITIALIZATION ==================
*/
t_shell	*initialize_shell(char **envp);
void	create_env_var_list(t_shell *shell, char **envp);

/*
** -- MODES --
*/
void	interactive_mode(t_shell *shell);
void	w_arg_mode(t_shell *shell, int argc, char **argv);
void	std_input_mode(int fd, t_shell *shell);
/*
** -- SIGNALS --
*/
void	sigint_handler(int sig_num);
void	setup_signals(t_shell *shell);

/*
** ================== PARSING ==================
*/
/*
** -- TOKENIZATION --
*/
void	create_tokens(t_shell *shell, const char *s);
void	addToken(t_shell *shell, const char *value, int type);
TokenType	get_token_type(const char *token_text);
char	*parse_tokens(t_shell *shell, const char *s);
/*
** -- QUOTES --
*/
void	toggleQuoteState(int *quoteState);
char	*reviewquotes(char *input);
int	shouldExpandVariable(const char *word);
/*
** -- VARIABLE EXPANSION --
*/
char	*expandVariables(t_shell *shell, const char *input);
char	*expandVariables2(t_shell *shell, const char *input,
		size_t *advancedPosition);
/*
** -- HEREDOC --
*/
void	parse_heredoc(t_shell *shell);
void prepare_heredocs_in_command_table(CommandTable* table);
void handle_heredoc_for_commands(t_shell *shell, CommandTable* table);
void cleanup_heredocs_in_command_table(CommandTable* table);
/*
** ================== COMMANDS ==================
*/
/*
** -- COMMAND TABLE --
*/
CommandTable	*create_command_table(t_shell *shell, TokenNode *tokens);
void	execute_command_table(t_shell *shell, CommandTable *table);
void	free_command_table(CommandTable *table);
// void	execute_ext_command(Command *cmd);
void execute_ext_command(t_shell *shell, Command *cmd);
/*
** -- BUILT_IN COMMANDS --
*/
int	builtin_cd(t_shell *shell, char **args, int n_args);
int	builtin_pwd(void);
int	builtin_echo(t_shell *shell, char **args, int n_args,Command* cmd);
int	builtin_unset(t_shell *shell, char **args, int n_args);
int	builtin_env(t_shell *shell);
/*
** :: EXIT ::
*/
int	builtin_exit(t_shell *shell, char **args, int n_args);
char	*export_exit_code(t_shell *shell);
int	is_valid_number(const char *str);
int	adjust_exit_code(int n);
/*
** :: EXPORT ::
*/
int	builtin_export(t_shell *shell, char **args, int n_args);
int	process_env_arg(t_shell *shell, const char *arg);
int	check_duplicates(t_shell *shell, const char *key, int nchar);
void	update_var(t_shell *shell, const char *key, const char *value);
void	add_new_var(t_shell *shell, const char *key, const char *value);
void	decl_new_var(t_shell *shell, const char *key);
void	remove_var(t_shell *shell, const char *key, int nchar);
void	print_export(t_shell *shell, int is_export);
/*
** ================== MEMORY ==================
*/
void	shexit(t_shell *shell, int exit_code);
void	*shell_malloc(t_shell *shell, size_t size);
void	shell_free(t_shell *shell, void *ptr);
void	shell_cleanup(t_shell *shell);
char	*shell_strdup(t_shell *shell, const char *s);
char	*shell_strndup(t_shell *shell, const char *s, size_t n);
/*
** ================== UTILS ==================
*/
const char	*skip_delimiters(const char *s, char c);
int	find_index_char(const char *str, char c);
int	intLength(int num);
int isNotEmpty(const char *str);

/*
** -- DEBUG --
*/
void	printTokens(TokenNode *head);
void	print_command_table(const CommandTable *table);
int is_token_type_present(TokenNode *head, int type);

// pipex_and pipex_util
# define ERR_INVALID_CMD " : command not found\n"
# define STDIN_FILENO 0
# define STDOUT_FILENO 1

char	*check_path(char **envp);
char	*locate_cmd(char **paths, char *cmd);
int find_env_var(t_env_var *list, const char *key);
char	*find_env_path(char **envp);
void	free_array(char **v);
char	*ft_strdup_ignore(const char *s, char ignore);
int	ft_strchr_count(const char *s, int c);
char	*ft_strjoin_nconst(char *s1, char *s2);
int	ft_puterr(char *s, int ret);

int	execute_command_pipex(int prev_pipe,Command *cmd,t_shell *shell);
char	**find_cmd_paths(char **envp);
char	**ft_split_cmd_args(char *s);
// int			run_cmd(char *cmd, char **envp);
// int	run_cmd(Command *command, char **envp, t_shell *shell);
int	run_cmd(Command *command, t_shell *shell);

// int pipex(t_in in,Command cmd,t_shell *shell);
int	pipex( Command *cmd, t_shell *shell);

#endif
