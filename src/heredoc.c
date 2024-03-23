/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 15:02:35 by toto              #+#    #+#             */
/*   Updated: 2024/02/26 15:56:55 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup_temp_file(char *tempFilePath) {
    if (tempFilePath) {
        // Assuming unlinking is deferred to this stage for any reason
        unlink(tempFilePath);
        free(tempFilePath);
    }
}

// Function to convert a long integer to a string
char *long_to_string(long value) {
    char *buffer = malloc(21); // Maximum length for a 64-bit integer is 20 digits + null terminator
    if (buffer == NULL) {
        return NULL; // Memory allocation failed
    }
    char *ptr = buffer + 20; // Start at the end of the buffer
    *ptr = '\0'; // Null terminator
    long n = value;
    if (n == 0) {
        // Handle zero explicitly, otherwise empty string
        *--ptr = '0';
    }
    while (n > 0) {
        *--ptr = '0' + (n % 10);
        n /= 10;
    }
    char *result = strdup(ptr); // Duplicate the relevant part of the buffer
    free(buffer); // Free the original buffer
    return result;
}

// Function to convert an integer to a string
char *int_to_string(int value) {
    return long_to_string(value); // Use the same function since int is a subset of long
}

char *construct_file_path(const char *timeStr, const char *counterStr) {
    const char *basePath = "/tmp/shell_heredoc_";
    int totalLength = strlen(basePath) + strlen(timeStr) + strlen(counterStr) + 2; // +2 for underscore and null terminator

    char *filePath = malloc(totalLength);
    if (filePath == NULL) {
        perror("malloc failed");
        return NULL;
    }

    strcpy(filePath, basePath);
    strcat(filePath, timeStr);
    strcat(filePath, "_");
    strcat(filePath, counterStr);

    return filePath;
}

int create_and_unlink_file(const char *filePath) {
    int fd = open(filePath, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd == -1) {
        perror("Cannot create temporary file for heredoc");
        return -1;
    }

    if (unlink(filePath) == -1) {
        perror("Failed to unlink temporary file");
        close(fd);
        return -1;
    }

    return fd;
}

int create_temp_file(char **tempFilePath) {
    static int counter = 0; // Static counter to ensure uniqueness within the same second
    time_t now = time(NULL); // Get the current time

    char *timeStr = long_to_string((long)now);
    char *counterStr = int_to_string(counter++);
    if (timeStr == NULL || counterStr == NULL) {
        if (timeStr) free(timeStr);
        if (counterStr) free(counterStr);
        return -1; // Conversion failed
    }

    *tempFilePath = construct_file_path(timeStr, counterStr);
    if (*tempFilePath == NULL) {
        free(timeStr);
        free(counterStr);
        return -1;
    }
    free(timeStr);
    free(counterStr);
    int fd = create_and_unlink_file(*tempFilePath);
    if (fd == -1) {
        free(*tempFilePath);
        *tempFilePath = NULL;
        return -1;
    }
    return fd;
}

void write_heredoc_to_file(int fd, const char* delimiter) {
    char *line;

    while ((line = readline("> ")) != NULL) {
        if (ft_strcmp(line, (char *)delimiter) == 0) {
            free(line);
            break; // Delimiter found, stop reading input
        }
        write(fd, line, strlen(line));
        write(fd, "\n", 1); // Add newline
        free(line); // Free readline-allocated buffer -> double free
    }
}

char* handle_heredoc(t_shell *shell, const char* delimiter)
{
	// ft_printf("handle heredoc\n");
    char *tempFilePath;
    int fd = create_temp_file(&tempFilePath);
    
	(void)shell;
    if (fd == -1) {
        return NULL; // Error message already printed by create_temp_file
    }
    
    write_heredoc_to_file(fd, delimiter);
    
    close(fd);
    
    return tempFilePath; // Caller is responsible for freeing this
}


void parse_heredoc(t_shell *shell)
{
    TokenNode *current = shell->token_head;
    TokenNode *previous = NULL;

	// ft_printf("parse_heredoc START\n");
    while (current != NULL)
	{
		// ft_printf("parse_heredoc while current = |%s|\n", current->token.value);
        if (previous)
			// ft_printf("parse_heredoc if previous OK |%s| previous->token.type %d\n", previous->token.value, previous->token.type);
        if (previous && previous->token.type == TOKEN_REDIR_HEREDOC)
		{
			// ft_printf("parse_heredoc: found\n");
            char *tempFile = handle_heredoc(shell, current->token.value);
            if (tempFile) {
                // Here, handle the association of tempFile with the command
                // This could involve setting the command's redirect_in field, for example
                // Depending on your cleanup strategy, you might free tempFile later
            }
        }
        previous = current;
        current = current->next;
    }
}

