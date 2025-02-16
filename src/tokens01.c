/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:07:09 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 16:10:42 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The function ft_create_word_tok creates a token representing a word in the input string str, 
starting from position *i. It calculates the length of the word n, 
accounting for character sequences inside single (') and double (") quotes, 
while ignoring spaces as separators. Once the word length is determined, 
it extracts the content and creates the appropriate token using ft_extract_word. 
Afterward, it updates the value of *i, moving the pointer to the end of the processed word. 
The function returns 0 on success or -1 if an error occurs. 
Its purpose is to correctly process words in the lexical analyzer.
*/

int	ft_create_word_tok(char *str, int *i, t_token **tokens, t_data *data)
{
	int	n;

	n = 0;
	while (str[*i + n] && str[*i + n] != ' ')
	{
		if (str[*i + n] == '\'')
			ft_skip_sq(&n, &str[*i]);
		else if (str[*i + n] == '\"')
		{
			n++;
			while (str[*i + n] && str[*i + n] != '\"')
				n++;
			n++;
		}
		else
		{
			while (str[*i + n] && !ft_strchr("\"\' ", str[*i + n]))
				n++;
		}
	}
	if (ft_extract_word(&str[*i], &n, tokens, data) == -1)
		return (-1);
	*i = *i + n;
	return (0);
}

/*
The function ft_extract_word creates a new token of type word (T_WORD) 
based on the string str with length *n, 
allocating memory for a new string word and copying the appropriate part into it. 
It then checks for the presence of the $ symbol using ft_check_for_dollar, 
followed by processing the word with the function ft_cross_word. 
If neither of these operations returns an error, it creates a new token 
and adds it to the list tokens. The function enables splitting words into tokens 
during lexical analysis, 
considering environment variables and special modifications.
*/
int	ft_extract_word(char *str, int *n, t_token **tokens, t_data *data)
{
	char	*word;
	int		value;

	word = malloc(sizeof(char) * (*n + 1));
	if (!word)
		return (ft_perror_message());
	ft_strlcpy(word, str, (*n) + 1);
	if (ft_check_for_dollar(&word, data) == -1)
		return (-1);
	value = ft_cross_word(&word, tokens);
	if (value == -1)
	{
		free(word);
		return (-1);
	}
	else if (value == 0)
		return (0);
	if (create_token(word, T_WORD, tokens) == -1)
	{
		free(word);
		return (-1);
	}
	return (0);
}

/*
The function ft_check_for_dollar analyzes the content of the string *word
for special characters: single quotes ('), the dollar sign ($), and double
quotes ("). When it finds any of these, it skips the content inside single
quotes, processes environment variables using ft_dollar, and interprets
content inside double quotes using ft_cross_dq. The function modifies the
content of *word according to the context, returning -1 in case of an error.
Its goal is to process and resolve environment variables and special sequences
in the analyzed text.
*/
int	ft_check_for_dollar(char **word, t_data *data)
{
	int	i;

	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '\'')
			ft_skip_sq(&i, *word);
		else if ((*word)[i] == '$')
		{
			if (ft_dollar(&i, word, data) == -1)
				return (-1);
		}
		else if ((*word)[i] == '\"')
		{
			if (ft_cross_dq (&i, word, data) == -1)
				return (-1);
		}
		else
			i++;
	}
	return (0);
}

/*
The function ft_cross_word processes a word by iterating over its characters. 
If it encounters a quote (single or double), it calls ft_clear_quote to remove the quote marks. 
If it encounters white spaces (spaces, tabs, etc.), it calls ft_cut_token to split the word into tokens. 
If an error occurs or the process finishes, it interrupts the operation accordingly. 
For other characters, it simply proceeds to the next one. The function ends by returning 1 if the processing was successful.
*/
int	ft_cross_word(char **word, t_token **tokens)
{
	int	i;
	int	value;

	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '\'' || (*word)[i] == '\"')
		{
			if (ft_clear_quote(&i, word, (*word)[i]) == -1)
				return (-1);
		}
		else if (ft_strchr(" \t\r\n\v\f", (*word)[i]))
		{
			value = ft_cut_token(&i, word, tokens);
			if (value == -1)
				return (-1);
			if (value == 0)
				return (0);
		}
		else
			i++;
	}
	return (1);
}

/*
The function ft_cut_token processes a word, creating its token. It begins by
allocating memory for a new string tok_str and copies part of *word to
this string. Then it calls create_token to create a token of type T_WORD.
If this fails, it returns an error and frees the memory. The function then
ignores white spaces until it encounters the next token or the end of the word.
If no more characters are found, it frees the memory and finishes. If it
encounters more characters, it allocates new memory for the rest of the word,
copies it into new_word, updates *word, and resets the index i to 0
to process the new word. The function returns 1 if the word is continued,
or 0 if there’s nothing more to process.
*/
int	ft_cut_token(int *i, char **word, t_token **tokens)
{
	char	*tok_str;
	char	*new_word;

	tok_str = malloc(sizeof(char) * (*i + 1));
	if (!tok_str)
		return (ft_perror_message());
	ft_strlcpy(tok_str, *word, *i + 1);
	if (create_token(tok_str, T_WORD, tokens) == -1)
		return (ft_perror_free(tok_str, NULL, NULL));
	while ((*word)[*i] && ft_strchr(" \t\r\n\v\f", (*word)[*i]))
		(*i)++;
	if (!(*word)[*i])
	{
		free(*word);
		return (0);
	}
	new_word = malloc(sizeof(char) * (ft_strlen(&(*word)[*i]) + 1));
	if (!new_word)
		return (ft_perror_message());
	ft_strlcpy(new_word, &(*word)[*i], (ft_strlen(&(*word)[*i]) + 1));
	free(*word);
	*word = new_word;
	*i = 0;
	return (1);
}
