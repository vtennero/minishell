/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:55:49 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/15 13:56:39 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void toggleQuoteState(int *quoteState)
{
    *quoteState = !(*quoteState);
}

void processCharacter(char c, char **output, int *isInSingleQuotes, int *isInDoubleQuotes) {
    if (c == '\'' && !(*isInDoubleQuotes)) {
        toggleQuoteState(isInSingleQuotes);
    } else if (c == '\"' && !(*isInSingleQuotes)) {
        toggleQuoteState(isInDoubleQuotes);
    } else {
        *(*output) = c;
        (*output)++;
    }
}

void transformQuotes(const char *input, char *output) {
    int isInSingleQuotes = 0, isInDoubleQuotes = 0;
    char *outputPtr = output;

    while (*input) {
        processCharacter(*input, &outputPtr, &isInSingleQuotes, &isInDoubleQuotes);
        input++;
    }
    *outputPtr = '\0'; // Null-terminate the output string
}

char 	*reviewquotes(char *input)
{
    char *output = (char *)malloc(strlen(input) + 1); // Allocate memory for the output string

    if (output == NULL) {
        printf("Memory allocation failed\n");
        return (NULL);
    }

    transformQuotes(input, output);
    // printf("reviewquotes Transformed string: %s\n", output);
	return (output);
}
