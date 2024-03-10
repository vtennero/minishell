/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magicmalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:42:10 by vitenner          #+#    #+#             */
/*   Updated: 2024/02/23 14:26:28 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char* shell_strdup(t_shell* shell, const char* s)
{
    size_t len = ft_strlen(s) + 1; // +1 for the null terminator
    char* newStr = (char*)shell_malloc(shell, len);
    if (newStr) {
        ft_strncpy(newStr, (char *)s, len);
        newStr[len - 1] = '\0'; // Ensure null termination
    }
    return newStr;
}

char* shell_strndup(t_shell* shell, const char* s, size_t n) {
    // Find the length of the string, ensuring not to read beyond its termination
    size_t len = strlen(s);

    // Ensure we do not copy more than n characters
    if (len > n) {
        len = n;
    }

    // Allocate enough memory for the string plus the null terminator
    char* newStr = (char*)shell_malloc(shell, len + 1);
    if (newStr) {
        // Copy up to len characters
        strncpy(newStr, s, len);
        // Explicitly null-terminate the new string
        newStr[len] = '\0';
    }
    return newStr;
}

void *shell_malloc(t_shell* shell, size_t size)
{
    void* ptr = ft_calloc(1, size);
    if (ptr) {
        MemNode* node = (MemNode*)malloc(sizeof(MemNode));
        if (node) {
            node->ptr = ptr;
            node->next = shell->mem_tracker.head;
            shell->mem_tracker.head = node;
        } else {
            // Handle failed allocation for MemNode
            free(ptr);
            return NULL;
        }
    }
    return ptr;
}

void shell_free(t_shell* shell, void* ptr)
{
    MemNode** current = &shell->mem_tracker.head;
    while (*current) {
        MemNode* entry = *current;
        if (entry->ptr == ptr) {
            *current = entry->next;
            free(entry->ptr);
            free(entry);
            return;
        }
        current = &entry->next;
    }
}

void shell_cleanup(t_shell* shell)
{
    MemNode* current = shell->mem_tracker.head;
    while (current) {
        MemNode* next = current->next;
        free(current->ptr);
        free(current);
        current = next;
    }
    shell->mem_tracker.head = NULL;
}

void	shexit(t_shell *shell, int exit_code)
{
    int exit_s;
    
    // ft_printf("shexit\n");
    exit_s = shell->last_exit_status;
    // ft_printf("%d\n", shell->last_exit_status);
	shell_cleanup(shell);
	// free big struct
	free(shell);
	// exit
	// exit(exit_code);
    exit(exit_s);
    (void)exit_code;
}
