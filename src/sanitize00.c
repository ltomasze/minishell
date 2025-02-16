/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize00.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:49:47 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:52:01 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_operator(char c);
static void	handle_operators(const char *str, char *str_final, int *i, int *j);
static void	handle_quotes(char *str, char *str_final, int *i, int *j);
static int	realloc_str_final(char **str_final,
				int j, int str_final_len);

/*
The `process_str` function processes the input string `str`  
and stores the updated version in `str_final`.  
It iterates through `str`, skipping unnecessary spaces  
and tabs, while handling special cases like quotes (`handle_quotes`)  
and operators (`handle_operators`).  
If the buffer `str_final` becomes too small,  
its size is increased using `realloc_str_final`.  
The function completes the processing by adding a null character (`\0`) at the end of the string and returns the result.  
Its purpose is to normalize and interpret the input string in the context of command syntax.
*/
char	*process_str(char *str, char *str_final,
int str_final_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (j + 1 >= str_final_len)
			str_final_len = realloc_str_final(&str_final, j, str_final_len);
		if (str_final_len == -1)
			return (NULL);
		if (str[i] == '"' || str[i] == '\'')
			handle_quotes(str, str_final, &i, &j);
		else if (ft_isspace(str[i]) && (i == 0 || ft_isspace(str[i + 1])))
			i++;
		else if
			((ft_isalnum(str[i]) && is_operator(str[i + 1]))
			|| (str[i] == '|' && (str[i + 1] == '$'
			|| str[i + 1] == '<' || str[i + 1] == '>' )))
			handle_operators(str, str_final, &i, &j);
		else
			str_final[j++] = str[i++];
	}
	str_final[j] = '\0';
	return (str_final);
}

/*
The is_operator function checks whether the character c
is one of the operators (>, <, |).
If so, it returns 1; otherwise, it returns 0.
It is used to recognize operators in the analyzed text.
*/
static int	is_operator(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	else
		return (0);
}

/*
The handle_quotes function processes strings enclosed in quotes within the str
string. It determines the start (i_cp) and end of the quote,
copying its contents to str_final.
It then updates the positions of the i and j pointers.
If the character after the quote is an operator,
it adds a space to str_final to separate the operator from the rest of the text.
This function helps manage correct processing of quotes in input data.
*/
static void	handle_quotes(char *str, char *str_final, int *i, int *j)
{
	int	i_cp;

	i_cp = *i;
	go_to_next_quote(str, i, false);
	(*i)++;
	ft_strlcpy(&str_final[*j], &str[i_cp], *i - i_cp + 1);
	*j += *i - i_cp;
	if (is_operator(str[*i]))
	{
		str_final[*j] = ' ';
		(*j)++;
		str_final[*j] = '\0';
	}
}

/*
The handle_operators function copies the operator from str to str_final,
moving the i and j pointers,
and then adds a space after the operator.
This helps correctly separate operators in the analyzed string.
*/
static void	handle_operators(const char *str, char *str_final, int *i, int *j)
{
	str_final[(*j)++] = str[(*i)++];
	str_final[(*j)++] = ' ';
}

/*
The realloc_str_final function doubles the size of the str_final buffer
to accommodate more data. It creates a new, larger buffer,
copies the current content to the new buffer,
frees the old buffer, and updates the pointer.
In case of a memory allocation error, it returns -1,
and upon success, it returns the new buffer length.
It is used for dynamic memory management for the growing string.
*/
static int	realloc_str_final(char **str_final,
int j, int str_final_len)
{
	char	*new_str;

	new_str = malloc(sizeof(char) * (str_final_len * 2));
	if (!new_str)
	{
		free(*str_final);
		return (-1);
	}
	ft_strlcpy(new_str, *str_final, j);
	free(*str_final);
	*str_final = new_str;
	return (str_final_len * 2);
}
