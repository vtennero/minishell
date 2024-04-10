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

char* create_temp_file_path(const char* basePath, const char* timeStr, const char* counterStr) {
    // Calculate required buffer size (+2 for underscore and null terminator)
    int bufferSize = strlen(basePath) + strlen(timeStr) + strlen(counterStr) + 2;
    char* tempFilePath = malloc(bufferSize);
    if (tempFilePath == NULL) {
        perror("malloc failed");
        return NULL;
    }

    // Pointer to current position in tempFilePath
    char *ptr = tempFilePath;

    // Copy the base path into tempFilePath and increment pointer by length of basePath
    memcpy(ptr, basePath, strlen(basePath));
    ptr += strlen(basePath);

    // Copy the time string and increment pointer by length of timeStr
    memcpy(ptr, timeStr, strlen(timeStr));
    ptr += strlen(timeStr);

    // Add the underscore and increment pointer by 1
    *ptr = '_';
    ptr++;

    // Copy the counter string
    memcpy(ptr, counterStr, strlen(counterStr) + 1); // +1 to include the null terminator

    return tempFilePath; // The caller is responsible for freeing this memory
}





int create_and_unlink_temp_file(const char* tempFilePath) {
    // Attempt to create the file exclusively with read and write permissions for the user
    int fd = open(tempFilePath, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd == -1) {
        perror("Cannot create temporary file for heredoc");
        return -1;
    }

    // Immediately unlink the file so it's removed upon closing
    // if (unlink(tempFilePath) == -1) {
    //     perror("Failed to unlink temporary file");
    //     close(fd);
    //     return -1;
    // }

    return fd;
}


int create_temp_file(char **tempFilePath) {
    static int counter = 0; // Static counter to ensure uniqueness within the same second
    char *timeStr = ft_itoa((int)time(NULL));
    char *counterStr = ft_itoa(counter++);

    if (timeStr == NULL || counterStr == NULL) {
        free(timeStr); // In case one succeeded but the other didn't
        free(counterStr);
        return -1;
    }

    *tempFilePath = create_temp_file_path("/tmp/", timeStr, counterStr);

    // Clean up
    free(timeStr);
    free(counterStr);

    if (*tempFilePath == NULL) {
        return -1;
    }

    int fd = create_and_unlink_temp_file(*tempFilePath);
    if (fd == -1) {
        free(*tempFilePath);
        *tempFilePath = NULL;
    }

    return fd;
}


// int create_temp_file(char **tempFilePath) {
//     static int counter = 0; // Static counter to ensure uniqueness within the same second
//     time_t now = time(NULL); // Get the current time
//     char *timeStr = ft_itoa((int)now);
//     char *counterStr = ft_itoa(counter++);

//     // Calculate required buffer size
//     int bufferSize = strlen("/tmp/shell_heredoc_") + strlen(timeStr) + strlen(counterStr) + 2; // +2 for underscore and null terminator
//     *tempFilePath = malloc(bufferSize);
//     if (*tempFilePath == NULL) {
//         perror("malloc failed");
//         free(timeStr);
//         free(counterStr);
//         return -1;
//     }

//     // Construct file path
//     strcpy(*tempFilePath, "/tmp/shell_heredoc_");
//     strcat(*tempFilePath, timeStr);
//     strcat(*tempFilePath, "_");
//     strcat(*tempFilePath, counterStr);

//     // Clean up
//     free(timeStr);
//     free(counterStr);

//     // Attempt to create the file exclusively with read and write permissions for the user
//     int fd = open(*tempFilePath, O_RDWR | O_CREAT | O_EXCL, 0600);
//     if (fd == -1) {
//         perror("Cannot create temporary file for heredoc");
//         free(*tempFilePath);
//         *tempFilePath = NULL;
//         return -1;
//     }

//     // Immediately unlink the file so it's removed upon closing
//     if (unlink(*tempFilePath) == -1) {
//         perror("Failed to unlink temporary file");
//         close(fd);
//         free(*tempFilePath);
//         *tempFilePath = NULL;
//         return -1;
//     }

//     return fd;
// }


void cleanup_temp_file(char *tempFilePath) {
    if (tempFilePath) {
        // Assuming unlinking is deferred to this stage for any reason
        unlink(tempFilePath);
        free(tempFilePath);
    }
}


void cleanup_heredocs_in_command_table(CommandTable* table)
{
    Command* cmd = table->head;
    while (cmd) {
        if (cmd->heredoc_temp_path) {
            // unlink(cmd->heredoc_temp_path);
            cleanup_temp_file(cmd->heredoc_temp_path);
            cmd->heredoc_temp_path = NULL; // Ensure the pointer is cleared after cleanup.
        }
        cmd = cmd->next;
    }
}

void write_heredoc_to_file(int fd, const char* delimiter) {
    char *line;

    while ((line = readline("> ")) != NULL) {
        if (ft_strcmp(line, delimiter) == 0) {
            free(line);
            break; // Delimiter found, stop reading input
        }
        write(fd, line, strlen(line));
        write(fd, "\n", 1); // Add newline
        free(line); // Free readline-allocated buffer -> double free
    }
}


char* handle_heredoc(const char* delimiter)
{
    char *tempFilePath;
    int fd = create_temp_file(&tempFilePath);
    if (fd == -1) {
        return NULL; // Error message already printed by create_temp_file
    }

    write_heredoc_to_file(fd, delimiter);
    close(fd);
    return tempFilePath; // Caller is responsible for freeing this
}

void prepare_heredocs_in_command_table(CommandTable* table)
{
    Command* cmd = table->head;
    while (cmd) {
        if (cmd->heredoc_delimiter) {
            cmd->heredoc_temp_path = handle_heredoc(cmd->heredoc_delimiter);
            cmd->fin=-9;
        }
        cmd = cmd->next;
    }
}
