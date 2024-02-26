/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 15:00:40 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/26 11:35:38 by toto             ###   ########.fr       */
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
    TOKEN_COMMAND,
    TOKEN_ARG,
    TOKEN_REDIR_IN,  // <
    TOKEN_REDIR_OUT, // >
    TOKEN_REDIR_APPEND, // >>
    TOKEN_REDIR_HEREDOC, // <<
    TOKEN_PIPE,     // |
    TOKEN_ENV_VAR,  // $VAR
    TOKEN_EXIT_STATUS, // $?
    TOKEN_D_Q,
    TOKEN_S_Q,
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


typedef struct s_shell {
	char**              env_vars;       	// Environment variables
	int                 last_exit_status;  	// Exit status of the last executed command
	struct sigaction    *signals; // Custom signal handlers
	int                 is_interactive;
    MemTracker          mem_tracker;
    TokenNode           *token_head;
} t_shell;


typedef struct {
	char* key;   // The name of the environment variable
	char* value; // The value of the environment variable
} EnvironmentVariable;


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
void builtin_cd(char** args, int n_args);
void builtin_pwd(void);
void builtin_echo(char** args, int n_args);
void builtin_export(char* variable, char* value);
void builtin_unset(char* variable);
void builtin_env(void);
void builtin_exit(t_shell *shell);


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

#endif
