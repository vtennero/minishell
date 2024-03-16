/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitenner <vitenner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:55:49 by vitenner          #+#    #+#             */
/*   Updated: 2024/03/16 16:19:39 by vitenner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int shouldExpandVariable(const char *word)
{
    int quoteCountleft = 0;
    int quoteCountright = 0;

    while (*word && *word == '\'')
    {
        quoteCountleft++;
        word++;
    }
    while (*word && *word != '\'')
        word++;
    while (*word && *word == '\'')
    {
        quoteCountright++;
        word++;
    }
    // ft_printf("quoteCountleft %d quoteCountright %d\n", quoteCountleft, quoteCountright);
    if (quoteCountleft % 2 == 0 && quoteCountright % 2 == 0)
        return (1);
    return (0); // Return 1 if even, 0 if odd
}

void toggleQuoteState(int *quoteState)
{
    *quoteState = !(*quoteState);
}

// void processCharacter(char c, char **output, int *isInSingleQuotes, int *isInDoubleQuotes) {
//     if (c == '\'' && !(*isInDoubleQuotes)) {
//         toggleQuoteState(isInSingleQuotes);
//     } else if (c == '\"' && !(*isInSingleQuotes)) {
//         toggleQuoteState(isInDoubleQuotes);
//     } else {
//         *(*output) = c;
//         (*output)++;
//     }
// }

void processCharacter(char c, char **output, int *isInSingleQuotes, int *isInDoubleQuotes, QuoteType *quoteType) {
    if (c == '\'' && !(*isInDoubleQuotes)) {
        toggleQuoteState(isInSingleQuotes);
        if (*isInSingleQuotes) *quoteType = SINGLE_QUOTES; // Enter single quotes
        else *quoteType = NO_QUOTES; // Exit single quotes
    } else if (c == '\"' && !(*isInSingleQuotes)) {
        toggleQuoteState(isInDoubleQuotes);
        if (*isInDoubleQuotes) *quoteType = DOUBLE_QUOTES; // Enter double quotes
        else *quoteType = NO_QUOTES; // Exit double quotes
    } else {
        *(*output) = c;
        (*output)++;
    }
}


void transformQuotes(const char *input, char *output, QuoteType *quoteType) {
    int isInSingleQuotes = 0, isInDoubleQuotes = 0;
    char *outputPtr = output;

    while (*input) {
        processCharacter(*input, &outputPtr, &isInSingleQuotes, &isInDoubleQuotes, quoteType);
        input++;
    }
    *outputPtr = '\0'; // Null-terminate the output string
}


char *reviewquotes(char *input, QuoteType *quoteType)
{
    char *output = (char *)malloc(strlen(input) + 1);
    // int singleQuoteCount = 0; // Initialize the quote counter

    if (output == NULL) {
        perror("Memory allocation failed\n");
        return (NULL);
    }

    transformQuotes(input, output, quoteType);
    // Now, singleQuoteCount contains the number of single quotes encountered
    // printf("reviewquotes Transformed string: %s\n", output);
    // printf("Single quotes encountered: %d\n", singleQuoteCount);

    return output;
}

