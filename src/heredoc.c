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

int create_temp_file(char **tempFilePath) {
    static int counter = 0; // Static counter to ensure uniqueness within the same second
    time_t now = time(NULL); // Get the current time

    // Allocate memory for the file path with enough space for the format
    *tempFilePath = malloc(sizeof(char) * (strlen("/tmp/shell_heredoc_") + 20 + 10)); // Timestamp can be up to 10 digits, counter up to 10, + null terminator
    if (*tempFilePath == NULL) {
        perror("malloc failed");
        return -1;
    }

    // Generate a unique file path using the timestamp and counter
    sprintf(*tempFilePath, "/tmp/shell_heredoc_%ld_%d", (long)now, counter++);

    // Attempt to create the file exclusively with read and write permissions for the user
    int fd = open(*tempFilePath, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd == -1) {
        perror("Cannot create temporary file for heredoc");
        free(*tempFilePath);
        *tempFilePath = NULL;
        return -1;
    }

    if (unlink(*tempFilePath) == -1) {
        perror("Failed to unlink temporary file");
        // Handle error (e.g., by closing the file descriptor and freeing memory)
        close(fd);
        free(*tempFilePath);
        *tempFilePath = NULL;
        return -1;
    }

    return fd;
}

void write_heredoc_to_file(int fd, const char* delimiter) {
    char *line;

    while ((line = readline("> ")) != NULL) {
        if (strcmp(line, delimiter) == 0) {
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
	ft_printf("handle heredoc\n");
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

	ft_printf("parse_heredoc START\n");
    while (current != NULL)
	{
		ft_printf("parse_heredoc while current = |%s|\n", current->token.value);
        if (previous)
			ft_printf("parse_heredoc if previous OK |%s| previous->token.type %d\n", previous->token.value, previous->token.type);
        if (previous && previous->token.type == TOKEN_REDIR_HEREDOC)
		{
			ft_printf("parse_heredoc: found\n");
            char *tempFile = handle_heredoc(shell, current->token.value);
            if (tempFile) {
                // Here, handle the association of tempFile with the command
                // This could involve setting the command's redirect_in field, for example
                printf("Heredoc temporary file created: %s\n", tempFile);
                // Depending on your cleanup strategy, you might free tempFile later
            }
        }
        previous = current;
        current = current->next;
    }
	ft_printf("parse_heredoc END\n");
}

