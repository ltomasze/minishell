/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_satinization.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:56:34 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:59:16 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The function ft_skip_sq skips over the content inside single quotes (') 
in the string str, starting from position *n. 
It first increments *n to skip the opening quote, 
then iterates until it encounters the closing quote or the end of the string. 
Finally, it increments *n again to move past the closing quote.
This function allows correct handling of the content inside single quotes when performing lexical analysis.
*/
void	ft_skip_sq(int *n, char *str)
{
	(*n)++;
	while (str[*n] && str[*n] != '\'')
		(*n)++;
	(*n)++;
}

/*
The function ft_cross_dq processes the string between double quotes, 
starting from index i+1. If it encounters the $ character, 
it calls the ft_dollar function to process the environment variable. 
In case of an error, it returns -1. After processing each character (other than $), 
the function proceeds to the next one until it reaches the closing quote. 
At the end, it increments i by 1 to point to the position after the closing quote and returns 0, 
indicating completion.
*/
int	ft_cross_dq(int *i, char **word, t_data *data)
{
	(*i)++;
	while ((*word)[*i] != '\"')
	{
		if ((*word)[*i] == '$')
		{
			if (ft_dollar(i, word, data) == -1)
				return (-1);
			continue ;
		}
		(*i)++;
	}
	(*i)++;
	return (0);
}

/*
The function ft_clear_quote removes quotes (either single or double) from a string, 
starting from position i. It searches for the position of the closing quote 
and creates a new string without that quote. The variable new_word is allocated 
for a new version of the string, where the fragment before and after the quote is copied, 
omitting the quote itself. After the memory allocation and copying are completed, 
the original string *word is freed, and the pointer *word is updated to point to the newly created version. 
The function returns 0 after successful processing, and in case of memory allocation failure, it calls ft_perror_message.
*/
int	ft_clear_quote(int *i, char **word, char del)
{
	int		n;
	char	*new_word;
	int		len;

	n = 1;
	len = ft_strlen(*word);
	while ((*word)[*i + n] && (*word)[*i + n] != del)
		n++;
	new_word = malloc(sizeof(char) * (len - 1));
	if (!new_word)
		return (ft_perror_message());
	ft_strlcpy(new_word, *word, *i + 1);
	ft_strlcpy(&new_word[*i], &(*word)[*i + 1], n);
	ft_strlcpy(&new_word[*i + n - 1], &(*word)[*i + n + 1], (len - *i - n));
	free(*word);
	*word = new_word;
	*i = *i + n - 1;
	return (0);
}
