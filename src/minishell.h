/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 15:00:40 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/15 14:48:21 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <signal.h>
// # include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>

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
    TOKEN_PIPE,     // |
    TOKEN_ENV_VAR,  // $VAR
    TOKEN_EXIT_STATUS, // $?
    // Add other necessary token types
} TokenType;

typedef struct Command {
	char* name;         	// Command name, e.g., "ls"
	char** args;        	// Arguments array, including the command itself as args[0]
	int argc;           	// Argument count
	char* input_redirect;   // File name for input redirection, NULL if not used
	char* output_redirect;  // File name for output redirection, NULL if not used
	int append;         	// Boolean indicating if output redirection should append
	int pipe[2];        	// File descriptors for piping, -1 if not used
} Command;

typedef enum {
	REDIRECT_NONE,  	// No redirection
	REDIRECT_INPUT, 	// Input redirection ('<')
	REDIRECT_OUTPUT,	// Output redirection ('>')
	REDIRECT_APPEND 	// Output append redirection ('>>')
} RedirectionType;


typedef struct {
	Command* commands;  // Array of Command structures
	int count;      	// Number of commands in the pipeline
} CommandTable;

typedef struct {
	char** env_vars;       	// Environment variables
	int last_exit_status;  	// Exit status of the last executed command
	struct sigaction* signals; // Custom signal handlers
	int	is_interactive;
} ShellState;


typedef struct {
	char* key;   // The name of the environment variable
	char* value; // The value of the environment variable
} EnvironmentVariable;

void ft_split_to_list(char const *s, char c, TokenNode** head);
void freeTokenList(TokenNode* head);
void printTokens(TokenNode* head);
void process_pipes(TokenNode** head);
void user_input_subfunction(TokenNode** head, TokenNode* last_pipe);

void ft_split_to_list(const char *s, char c, TokenNode **head);

// singals
void sigint_handler(int sig_num);
void sigquit_handler(int sig_num);

#endif
