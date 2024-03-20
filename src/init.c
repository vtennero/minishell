/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:46:06 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/20 11:05:25 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// v2 testing 
int split_key_value(const char *str, char **key, char **value) {
    const char *equals_pos = strchr(str, '=');
    if (equals_pos != NULL) {
        int key_len = equals_pos - str;
        *key = (char *)malloc(key_len + 1); // +1 for the null terminator
        strncpy(*key, str, key_len);
        (*key)[key_len] = '\0'; // Ensure null termination

        *value = strdup(equals_pos + 1); // Copy everything after '='
        return 1; // Success
    }
    // '=' not found
    return 0;
}

void insert_sorted_env_var(t_shell *shell, char *key, char *value) {
    t_env_var *new_var = malloc(sizeof(t_env_var));
    new_var->key = key;
    new_var->value = value;
    new_var->next = NULL;

    // If list is empty or the new key should be before the first key
    if (shell->env_head == NULL || strcmp(key, shell->env_head->key) < 0) {
        new_var->next = shell->env_head;
        shell->env_head = new_var;
    } else {
        // Find the correct spot in the list
        t_env_var *current = shell->env_head;
        while (current->next != NULL && strcmp(current->next->key, key) < 0) {
            current = current->next;
        }
        // Insert the new variable
        new_var->next = current->next;
        current->next = new_var;
    }
}

void create_env_var_list(t_shell *shell, char **envp) {
    shell->env_head = NULL;
    char *key, *value;
    for (int i = 0; envp[i] != NULL; i++) {
        if (split_key_value(envp[i], &key, &value)) { // Split each string into key and value
            insert_sorted_env_var(shell, key, value);
        }
    }
}


// v1 works
// Splits a string into key and value at the first '=' character.
// Returns 1 if successful, 0 otherwise.
// int split_key_value(const char *str, char **key, char **value) {
//     const char *equals_pos = strchr(str, '=');
//     if (equals_pos != NULL) {
//         int key_len = equals_pos - str;
//         *key = (char *)malloc(key_len + 1); // +1 for the null terminator
//         strncpy(*key, str, key_len);
//         (*key)[key_len] = '\0'; // Ensure null termination

//         *value = strdup(equals_pos + 1); // Copy everything after '='
//         return 1; // Success
//     }
//     // '=' not found
//     return 0;
// }

// void create_env_var_list(t_shell *shell, char **envp)
// {
//     t_env_var *last = NULL;
//     char *key, *value;
//     for (int i = 0; envp[i] != NULL; i++) {
//         if (split_key_value(envp[i], &key, &value)) { // Split each string into key and value
//             t_env_var *new_var = malloc(sizeof(t_env_var));
//             new_var->key = key;  // Already strdup'ed by split_key_value
//             new_var->value = value; // Same here
//             new_var->next = NULL;
//             if (last == NULL) { // First element
//                 shell->env_head = new_var;
//             } else {
//                 last->next = new_var;
//             }
//             last = new_var; // Update the last element
//         }
//     }
// }

t_shell *initialize_shell(char **envp)
{
    // ft_printf("initialize_shell\n");

    t_shell* shell = (t_shell*)ft_calloc(1, sizeof(t_shell));
    if (!shell) {
        perror("Failed to initialize shell");
        exit(EXIT_FAILURE);
    }
    // configure_terminal();
    // Initialize the memory tracker within the shell
    shell->mem_tracker.head = NULL;
    shell->token_head = NULL;
    // change this!
    shell->last_exit_status = 0;
    shell->is_interactive = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
    // Initialize environment variables from envp, signal handlers, etc.
    // shell->env_vars = copy_env_vars(envp); // Example initialization, you'll need to implement copy_env_vars or similar
    setup_signals(shell); // Example function to setup custom signal handlers
    (void)envp;
    // initialize_shell_env(shell, envp);
    create_env_var_list(shell, envp);
    return shell;
}
