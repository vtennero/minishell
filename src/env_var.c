/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:20:29 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/12 18:36:04 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// process_env_arg_helpers

int starts_with_eq(const char *str) {
	printf("Invalid variable declaration: Starts with '='\n");
    return str[0] == '=';
}

int is_alloc_str(const char *str) {
    const char *equals_pos = strchr(str, '=');
	printf("Valid variable assignment\n");
    // Check if '=' is found and it is not the last character in the string
    return equals_pos != NULL && *(equals_pos + 1) != '\0';
}

int is_decl_str(const char *str) {
	printf("Declaration without assignment\n");
    return strchr(str, '=') == NULL;
}

int is_incompl_alloc_str(const char *str) {
    const char *equals_pos = strchr(str, '=');
    // Check if '=' is found and it is the last character in the string
	printf("Invalid variable declaration: Incomplete assignment\n");
    return equals_pos != NULL && *(equals_pos + 1) == '\0';
}

int process_env_arg(const char *arg)
{
    // First, check if the argument is valid
    if (starts_with_eq(arg))
        return 0; // Indicate invalid argument
    if (is_incompl_alloc_str(arg))
	{
		// check dup
		// clears string, allocates empty string to node
		// creates node with empty string malloced
        return 0; // Indicate invalid argument
    }
    if (is_decl_str(arg))
	{
		// check dup
		// do nothing
		// create a new node but without mallocing
        return 1; // Valid as a declaration, but no action needed
    }
    if (is_alloc_str(arg))
	{
		// check dup
		// update
		// create new node
        return 2; // Valid and requires assignment action
    }
    // If none of the conditions are met, it's an unrecognized format
    printf("Unrecognized or unsupported variable format\n");
    return 0; // Indicate invalid or unsupported argument
}


// process_env_arg_helpers END



// Splits a string into key and value at the first '=' character.
// Returns 1 if successful, 0 otherwise.
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


void create_env_var_list(t_shell *shell, char **envp)
{
    t_env_var *last = NULL;
    char *key, *value;
    for (int i = 0; envp[i] != NULL; i++) {
        if (split_key_value(envp[i], &key, &value)) { // Split each string into key and value
            t_env_var *new_var = malloc(sizeof(t_env_var));
            new_var->key = key;  // Already strdup'ed by split_key_value
            new_var->value = value; // Same here
            new_var->next = NULL;
            if (last == NULL) { // First element
                shell->env_head = new_var;
            } else {
                last->next = new_var;
            }
            last = new_var; // Update the last element
        }
    }
}


int check_duplicates(t_shell *shell, const char *key) {
    t_env_var *current = shell->env_head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return 1; // Found a duplicate
        }
        current = current->next;
    }
    return 0; // No duplicates found
}

void add_new_var(t_shell *shell, const char *key, const char *value)
{
    if (check_duplicates(shell, key)) {
        // printf("Duplicate variable: %s\n", key);
        return; // Avoid adding duplicates
    }

    t_env_var *new_var = malloc(sizeof(t_env_var));
    new_var->key = strdup(key);
    new_var->value = strdup(value);
    new_var->next = NULL;

    // Insert in alphabetical order
    t_env_var **tracer = &shell->env_head;
    while (*tracer && strcmp((*tracer)->key, key) < 0) {
        tracer = &(*tracer)->next;
    }
    new_var->next = *tracer;
    *tracer = new_var;
}

void remove_var(t_shell *shell, const char *key) {
    t_env_var **tracer = &shell->env_head;
    while (*tracer) {
        if (strcmp((*tracer)->key, key) == 0) {
            t_env_var *to_delete = *tracer;
            *tracer = (*tracer)->next;
            free(to_delete->key);
            free(to_delete->value);
            free(to_delete);
            return;
        }
        tracer = &(*tracer)->next;
    }
    printf("Variable not found: %s\n", key);
}

void update_var(t_shell *shell, const char *key, const char *value) {
    t_env_var *current = shell->env_head;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            free(current->value); // Free old value
            current->value = strdup(value); // Assign new value
            return;
        }
        current = current->next;
    }
    printf("Variable not found: %s. Adding it instead.\n", key);
    add_new_var(shell, key, value);
}

void list_all_variables(t_shell *shell)
{
    t_env_var *current = shell->env_head;
    while (current) {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}
