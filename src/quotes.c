/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 13:55:49 by vitenner          #+#    #+#             */
/*   Updated: 2024/04/05 11:25:46 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shouldExpandVariable(const char *word)
{
	int	quoteCountleft;
	int	quoteCountright;

	quoteCountleft = 0;
	quoteCountright = 0;
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
		if (quoteCountleft % 2 == 0 && quoteCountright % 2 == 0)
			return (1);
		return (0); // Return 1 if even, 0 if odd
}

void	toggleQuoteState(int *quoteState)
{
	*quoteState = !(*quoteState);
}

void	processCharacter(char c, char **output, int *isInSingleQuotes,
		int *isInDoubleQuotes)
{
	if (c == '\'' && !(*isInDoubleQuotes))
	{
		toggleQuoteState(isInSingleQuotes);
	}
	else if (c == '\"' && !(*isInSingleQuotes))
	{
		toggleQuoteState(isInDoubleQuotes);
	}
	else
	{
		*(*output) = c;
		(*output)++;
	}
}

void	transformQuotes(const char *input, char *output)
{
	int		isInSingleQuotes;
	int		isInDoubleQuotes;
	char	*outputPtr;

	isInSingleQuotes = 0;
	isInDoubleQuotes = 0;
	outputPtr = output;
	while (*input)
	{
		processCharacter(*input, &outputPtr, &isInSingleQuotes,
			&isInDoubleQuotes);
		input++;
	}
	*outputPtr = '\0'; // Null-terminate the output string
}

char	*reviewquotes(char *input)
{
	char	*output;

	output = (char *)malloc(strlen(input) + 1);
	if (output == NULL)
	{
		perror("Memory allocation failed\n");
		return (NULL);
	}
	transformQuotes(input, output);
	return (output);
}
