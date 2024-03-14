/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 15:00:40 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/26 15:05:30 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <signal.h>
# include <stdio.h>
# include <fcntl.h>
// # include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
#include <errno.h>

// memory management
typedef struct MemNode {
    void* ptr;                // Pointer to the allocated memory
    struct MemNode* next;     // Next node in the list
} MemNode;

typedef struct {
    MemNode* head;            // Head of the list of allocated memory blocks
} MemTracker;

// 

typedef struct {
	char* value; // The actual token, e.g., "ls", "-l", ">", etc.
	int type;	// Type of the token (command, argument, redirection, pipe, etc.)
} Token;

typedef struct TokenNode {
    Token token;
    struct TokenNode* next;
} TokenNode;

typedef enum {
    TOKEN_COMMAND,          // 0
    TOKEN_ARG,              // 1
    TOKEN_REDIR_IN, // <       2
    TOKEN_REDIR_OUT,// >       3
    TOKEN_REDIR_APPEND, // >>  4
    TOKEN_REDIR_HEREDOC, // << 5
    TOKEN_PIPE,     // |        6
    TOKEN_ENV_VAR,  // $VAR     7
    TOKEN_EXIT_STATUS, // $?    8
    TOKEN_D_Q,  //              9
    TOKEN_S_Q,  //              10
    TOKEN_S_DOL,//              11
    // Add other necessary token types
} TokenType;

typedef enum {
    CMD_BUILTIN,
    CMD_EXTERNAL
} CommandType;

typedef struct Command {
    CommandType type;
    char* name;
    char** args;
    int arg_count;
    char* redirect_in;
    char* redirect_out;
    char* redirect_append;
    char* redirect_heredoc;
    int fd_in;                 // File descriptor for input redirection
    int fd_out;                // File descriptor for output redirection
    struct Command* next;
} Command;

typedef struct {
    Command*    head;             // Head of the list of commands
    int         command_count;         // Number of commands in the table
} CommandTable;

typedef enum {
	REDIRECT_NONE,  	// No redirection
	REDIRECT_INPUT, 	// Input redirection ('<')
	REDIRECT_OUTPUT,	// Output redirection ('>')
	REDIRECT_APPEND 	// Output append redirection ('>>')
} RedirectionType;

typedef struct s_env_var {
    char *key;                 // For storing the variable name
    char *value;               // For storing the variable value
    struct s_env_var *next;    // Pointer to the next node in the list
} t_env_var;

typedef struct s_shell {
    t_env_var           *env_head;       // Head of the linked list of environment variables for export
	int                 last_exit_status;  	// Exit status of the last executed command
	struct sigaction    *signals; // Custom signal handlers
	int                 is_interactive;
    MemTracker          mem_tracker;
    TokenNode           *token_head;
} t_shell;


// typedef struct {
// 	char* key;   // The name of the environment variable
// 	char* value; // The value of the environment variable
// } EnvironmentVariable;



// void create_tokens(t_shell *shell, const char *s, char c, TokenNode **head);
void create_tokens(t_shell *shell, const char *s);

// singals
void sigint_handler(int sig_num);
void sigquit_handler(int sig_num);
void setup_signals(t_shell *shell);

// commands
CommandTable* create_command_table(t_shell *shell, TokenNode* tokens);
void free_command_table(CommandTable* table);


void execute_command_table(t_shell *shell, CommandTable* table);


// builtins
void builtin_cd(t_shell * shell, char** args, int n_args);
void builtin_pwd(void);
void builtin_echo(t_shell *shell, char** args, int n_args);
void builtin_export(t_shell *shell, char** args, int n_args);
void builtin_unset(t_shell *shell, char** args, int n_args);
void builtin_env(void);
void builtin_exit(t_shell *shell, char** args, int n_args);


void execute_ext_command(Command *cmd);


// exit
void	shexit(t_shell *shell, int exit_code);


// shell memory allocation
void* shell_malloc(t_shell* shell, size_t size);
void shell_free(t_shell* shell, void* ptr);
void shell_cleanup(t_shell* shell);
char* shell_strdup(t_shell* shell, const char* s);
char* shell_strndup(t_shell* shell, const char* s, size_t n);
void expand_variables(t_shell *shell);

// debug
void printTokens(TokenNode* head);
void print_command_table(const CommandTable* table);

// heredoc
void parse_heredoc(t_shell *shell);

// env var
void create_env_var_list(t_shell *shell, char **envp);
int process_env_arg(t_shell *shell, const char *arg);
int check_duplicates(t_shell *shell, const char *key, int nchar);
void update_var(t_shell *shell, const char *key, const char *value);
void add_new_var(t_shell *shell, const char *key, const char *value);
void decl_new_var(t_shell *shell, const char *key);
void remove_var(t_shell *shell, const char *key, int nchar);
int find_index_char(const char *str, char c);
void print_export(t_shell *shell);

// export quotes
char *reviewquotes(char *input);

// export var expansion
char* expandVariables(const char *input, t_env_var *envVars);

#endif
