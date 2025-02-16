/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens00.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 15:09:41 by mbany             #+#    #+#             */
/*   Updated: 2025/02/16 16:06:44 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_single_redirection(char x, t_token **tokens, char *str);

/*
The function ft_append_redir checks if the character at input[*i] is either < or >, 
and then assigns the appropriate value "<<" or ">>" to the variable str. 
If memory allocation for str fails, it returns an error. 
The function creates a token of type T_HEREDOC or T_APPEND using the create_token function, 
and then advances the index *i by 2. 
If an error occurs while creating the token, it returns -1. 
The purpose of this function is to handle different types of redirection (<< or >>).
*/
static int	ft_append_redir(char *input, int *i, t_token **tokens, char *str)
{
	int	error;

	if (input [*i] == '<')
		str = ft_strdup("<<");
	else
		str = ft_strdup(">>");
	if (!str)
		return (ft_perror_message());
	if (input[*i] == '<')
		error = create_token(str, T_HEREDOC, tokens);
	else
		error = create_token(str, T_APPEND, tokens);
	if (error == -1)
	{
		free(str);
		return (-1);
	}
	*i = *i + 2;
	return (0);
}

/*
The function ft_is_redir checks if the character in the input string at index *i 
is a redirection character (< or >). 
In the case of a double redirection character (<< or >>), 
it calls ft_append_redir to create the appropriate token. 
For single redirection characters, it creates a token using ft_single_redirection 
and advances the index *i. In case of an error, it returns -1; otherwise, 
it returns 0, allowing the token analysis to continue. 
The function identifies redirections and adds them to the token list.
*/
int	ft_is_redir(char *input, int *i, t_token **tokens)
{
	int		error;
	char	*str;

	str = NULL;
	if (ft_strchr("<>", input[*i]))
	{
		if (input[*i] == input [*i + 1])
		{
			if (ft_append_redir(input, i, tokens, str) == -1)
				return (-1);
			return (0);
		}
		error = ft_single_redirection(input [*i], tokens, str);
		if (error != 0)
			return (-1);
		(*i)++;
		return (0);
	}
	return (0);
}

/*
The function ft_is_pipe checks if the character at the given position 
in the input string (input) (index indicated by *i) is the | character. 
If so, it copies this character into a newly created string str using ft_strdup, 
and then creates a new token of type T_PIPE, passing it to the token list 
using create_token. In case of errors (e.g., memory allocation failure 
when creating str or failure in create_token), the function handles the error 
by returning the appropriate value (-1). When the token is created successfully, 
the function increments the index *i (skipping the processed character in input) 
and returns 0. 
The function works to identify the | character as a pipeline operator during the tokenization process.
*/
int	ft_is_pipe(char *input, int *i, t_token **tokens)
{
	int		error;
	char	*str;

	if (input[*i] == '|')
	{
		str = ft_strdup("|");
		if (!str)
			return (ft_perror_message());
		error = create_token(str, T_PIPE, tokens);
		if (error == -1)
		{
			free(str);
			return (-1);
		}
		(*i)++;
	}
	return (0);
}

/*
The function ft_is_word checks if the character at the given position 
in the input string (input) at index *i is not one of the special characters |, <, >, or a space. 
If the condition is met, it calls the function ft_create_word_tok, 
which creates a token of type "word" and adds it to the token list. 
In case of an error in ft_create_word_tok, the function frees the memory allocated 
for input and returns -1 to signal failure. Otherwise, the function returns 0. 
Its goal is to identify and tokenize regular words during the lexical analysis of the input.
*/
int	ft_is_word(char *input, int *i, t_token **tokens, t_data *data)
{
	if (!(ft_strchr("| <>", input[*i])))
	{
		if (ft_create_word_tok(input, i, tokens, data) == -1)
		{
			free(input);
			return (-1);
		}
	}
	return (0);
}

/*
The function ft_single_redirection handles single input (<) or output (>) 
redirection characters by creating the appropriate token. 
Based on the x character, it assigns the correct type (T_IN_REDIR or T_OUT_REDIR) 
and copies the character into a new string using ft_strdup. 
It then calls create_token to add the new token to the tokens list. 
The function returns 0 if successful or -1 if an error occurs (e.g., out of memory). 
Its goal is to correctly recognize and add tokens for redirection operators in the lexical analyzer.
*/
static int	ft_single_redirection(char x, t_token **tokens, char *str)
{
	int	type;
	int	error;

	if (x == '<')
	{
		type = T_IN_REDIR;
		str = ft_strdup("<");
	}
	else
	{
		type = T_OUT_REDIR;
		str = ft_strdup(">");
	}
	if (!str)
		return (ft_perror_message());
	error = create_token(str, type, tokens);
	if (error == -1)
	{
		free(str);
		return (-1);
	}
	return (0);
}
