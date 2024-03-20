/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externalcommands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:31:07 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/18 14:32:07 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int find_env_var(t_env_var *list, const char *key)
{
    while (list != NULL) {  // Iterate through the list
        if (ft_strcmp(list->key, (char *)key) == 0) {  // Compare current node's key with the search key
            return 1;  // Key found
        }
        list = list->next;  // Move to the next node
    }
    return 0;  // Key not found
}

void execute_ext_command(t_shell *shell, Command *cmd)
{
    pid_t pid = fork();

    if (!find_env_var(shell->env_head, "PATH"))
    {
        perror("Command not found");
        shell->last_exit_status = 127;
        return;
    }
    if (pid == 0) { // Child process
        // Handle input redirection
        if (cmd->redirect_in != NULL) {
            cmd->fd_in = open(cmd->redirect_in, O_RDONLY);
            if (cmd->fd_in < 0) {
                perror("Failed to open input file");
                exit(EXIT_FAILURE);
            }
            dup2(cmd->fd_in, STDIN_FILENO);
            close(cmd->fd_in);
        }

        // Handle output redirection
        if (cmd->redirect_out != NULL) {
            cmd->fd_out = open(cmd->redirect_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (cmd->fd_out < 0) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            dup2(cmd->fd_out, STDOUT_FILENO);
            close(cmd->fd_out);
        }

        // Handle append redirection
        if (cmd->redirect_append != NULL) {
            cmd->fd_out = open(cmd->redirect_append, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (cmd->fd_out < 0) {
                perror("Failed to open append file");
                exit(EXIT_FAILURE);
            }
            dup2(cmd->fd_out, STDOUT_FILENO);
            close(cmd->fd_out);
        }

        // Execute command
        execvp(cmd->name, cmd->args);
        // If execvp returns, it means an error occurred
        perror("execvp");
        // exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("fork");
        // exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for child process to finish
    }
}
