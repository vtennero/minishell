/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:20:29 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/16 18:09:24 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// process_env_arg_helpers

int is_valid_var_name(const char *str, int n)
{
    int foundLetter = 0; // Flag to track if at least one letter has been found

    for(int i = 0; i < n; i++) {
        if(str[i] == '\0') { // If end of string is reached before n
            break;
        }
        if(!ft_isalpha(str[i]) && !ft_isdigit(str[i])) { // If not a letter or digit
            return 0; // Return 0 if any character doesn't meet the criteria
        }
        if(ft_isalpha(str[i])) { // If the character is a letter
            foundLetter = 1; // Set the flag
        }
    }
    return foundLetter; // Return the flag value (0 if only numbers, 1 otherwise)
}

int is_alloc_str(const char *str) {
    const char *equals_pos = strchr(str, '=');
    // Check if '=' is found and it is not the last character in the string
    return equals_pos != NULL && *(equals_pos + 1) != '\0';
}

int handle_incompl_alloc_str(t_shell *shell, const char *key, int nchar)
{
    char    *tmp1;

    tmp1 = shell_strndup(shell, key, nchar);
    if (check_duplicates(shell, key, nchar))
    {
        // ft_printf("handle_incompl_alloc_str: duplicate var found\n");
        update_var(shell, tmp1, "");
        // update_var(shell, tmp1, NULL);
    }
    else
    {
        // ft_printf("handle_incompl_alloc_str: no duplicate var found\n");
        add_new_var(shell, tmp1, NULL);
    }
    return (1);
}

int handle_decl_str(t_shell *shell, const char *key, int nchar)
{
    if (check_duplicates(shell, key, nchar))
    {
        // ft_printf("handle_decl_str: duplicate var found\n");
        // do nothing
    }
    else
    {
        // ft_printf("handle_decl_str: no duplicate var found\n");
        decl_new_var(shell, key);
    }
    return (1);
}

int handle_alloc_str(t_shell *shell, const char *key, int nchar)
{
    char    *tmp1;
    char    *tmp2;

    tmp1 = shell_strndup(shell, key, nchar);
    tmp2 = shell_strdup(shell, key + nchar + 1);
    if (check_duplicates(shell, key, nchar))
    {
        // ft_printf("handle_alloc_str: duplicate var found\n");
        update_var(shell, tmp1, tmp2);
    }
    else
    {
        // ft_printf("handle_alloc_str: no duplicate var found\n");
        add_new_var(shell, tmp1, tmp2);
    }
    return (1);
}

int process_env_arg(t_shell *shell, const char *arg)
{
    // First, check if the argument is valid
    int nchar;
    // char    *processedQuotes;
    // char    *wvarexpanded;

    // ft_printf("process_env_arg: |%s|\n", arg);
    nchar = find_index_char(arg, '=');
    // ft_printf("process_env_arg nchar %d strlen %d\n", nchar, (int)ft_strlen(arg) - 1);
    if (nchar == 0 || is_valid_var_name(arg, nchar) == 0)
    // if (nchar == 0 || arg[0] == '$')
    {
        shell->last_exit_status = 1;
        perror(" not a valid identifier");
        // first char is =
	    // printf("Invalid variable declaration: Starts with '='\n");
        return 0; // Indicate invalid argument
    }
    else if (nchar == (int)ft_strlen(arg) - 1)
	{
        // first = is last char of arg
	    // ft_printf("Invalid variable declaration: Incomplete assignment\n");
        handle_incompl_alloc_str(shell, arg, nchar);
        return 0; // Indicate invalid argument
    }
    else if (nchar == -1)
	{
        // no = in the string
	    // printf("Declaration without assignment\n");
        handle_decl_str(shell, arg, nchar);
        return 1; // Valid as a declaration, but no action needed
    }
    else if (is_alloc_str(arg))
	{
	    // printf("Valid variable assignment\n");
        // processedQuotes = reviewquotes(strdup(arg + nchar + 1));
        // wvarexpanded = expandVariables(processedQuotes, shell->env_head);
        // ft_printf("with variables expanded with |%s| %s\n", processedQuotes, wvarexpanded);
        // handle_alloc_str(shell, wvarexpanded + nchar + 1, ft_strlen(wvarexpanded));
        handle_alloc_str(shell, arg, nchar);
        return 2; // Valid and requires assignment action
    }
    else
    // If none of the conditions are met, it's an unrecognized format
    // ft_printf("Unrecognized or unsupported variable format\n");
    return 0; // Indicate invalid or unsupported argument
}


// process_env_arg_helpers END
int check_duplicates(t_shell *shell, const char *key, int nchar)
{
    t_env_var *current = shell->env_head;
    while (current != NULL) {
        if (ft_strncmp(current->key, key, nchar) == 0) {
            return 1; // Found a duplicate
        }
        current = current->next;
    }
    return 0; // No duplicates found
}

void add_new_var(t_shell *shell, const char *key, const char *value)
{
    t_env_var *new_var = malloc(sizeof(t_env_var));
    new_var->key = strdup(key);
    if (value)
        new_var->value = strdup(value);
    else
        new_var->value = strdup("\0");
    new_var->next = NULL;

    // Insert in alphabetical order
    t_env_var **tracer = &shell->env_head;
    while (*tracer && strcmp((*tracer)->key, key) < 0) {
        tracer = &(*tracer)->next;
    }
    new_var->next = *tracer;
    *tracer = new_var;
}

void decl_new_var(t_shell *shell, const char *key)
{
    t_env_var *new_var = malloc(sizeof(t_env_var));
    new_var->key = strdup(key);
    new_var->value = NULL;
    new_var->next = NULL;

    // Insert in alphabetical order
    t_env_var **tracer = &shell->env_head;
    while (*tracer && strcmp((*tracer)->key, key) < 0) {
        tracer = &(*tracer)->next;
    }
    new_var->next = *tracer;
    *tracer = new_var;
}

void remove_var(t_shell *shell, const char *key, int nchar)
{
    t_env_var **tracer = &shell->env_head;
    // ft_printf("remove_var\n");
    while (*tracer) {

        if (ft_strncmp((*tracer)->key, key, nchar) == 0) {
            // ft_printf("variable to unset found\n");
            t_env_var *to_delete = *tracer;
            *tracer = (*tracer)->next;
            (void)to_delete;
            free(to_delete->key);
            free(to_delete->value);
            free(to_delete);
            return;
        }
        tracer = &(*tracer)->next;
    }
    // printf("Variable not found: %s\n", key);
}

void update_var(t_shell *shell, const char *key, const char *value)
{
    t_env_var *current = shell->env_head;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            // ft_printf("update_var: im in the duplicat enow current->key %s, current->value %s\n", current->key, current->value);
            // free(current->value); // Free old value
            current->value = NULL;
            if (value)
                current->value = strdup(value); // Assign new value
            return;
        }
        current = current->next;
    }
}

void print_export(t_shell *shell)
{
    // ft_printf("print_export\n");
    t_env_var *current = shell->env_head;
    while (current) {
        if (shell->is_interactive == 1)
            ft_printf("declare -x ");
        ft_printf("%s", current->key);
        if (current->value !=NULL)
            ft_printf("=\"%s\"", current->value);
        ft_printf("\n");
        current = current->next;
    }
}

