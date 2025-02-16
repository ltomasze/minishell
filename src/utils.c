/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:11:01 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 16:12:18 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The go_to_next_quote function finds the position of the next or previous
occurrence of a quotation mark in the string line, depending on the value
of go_back. If go_back is false, it searches the string forward, and
if true, it searches backward. It saves the new position of the quotation
mark in the pointer i. The function handles the identification of quote
pairs in the text.
*/
void	go_to_next_quote(char *line, int *i, bool go_back)
{
	char	quote;
	int		j;

	if (!line || !i)
		return ;
	quote = line[*i];
	j = *i;
	if (go_back == false)
	{
		j++;
		while (line[j] != '\0' && line[j] != quote)
			j++;
	}
	else
	{
		j--;
		while (j >= 0 && line[j] != quote)
			j--;
	}
	*i = j;
}

/*
The ft_check_access function checks access to the file
file in read (READ) or write (WRITE) mode using the access function.
If the file does not exist or lacks the appropriate permissions,
it prints an error message and returns a negative value.
On success, it returns 0. This function is used
to verify the availability of files before performing operations on them.
*/
int	ft_check_access(char *file, int type)
{
	int	value;

	if (type == READ)
	{
		value = access(file, F_OK | R_OK);
		if (value < 0)
			return (ft_perror_message());
	}
	else if (type == WRITE)
	{
		value = access(file, F_OK | W_OK);
		if (value < 0)
			return (ft_perror_message());
	}
	return (0);
}
