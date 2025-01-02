/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitation1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:58:05 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/02 15:01:17 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_operator(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	else
		return (0);
}

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

static void	handle_operators(const char *str, char *str_final, int *i, int *j)
{
	str_final[(*j)++] = str[(*i)++];
	str_final[(*j)++] = ' ';
}

static int	realloc_str_final(char **str_final, int j, int str_final_len)
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

char	*process_str(char *str, char *str_final, int str_final_len)
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
		else if (ft_isspace(str[i]) && (i == 0 || ft_isspace(str[i - 1])))
			i++;
		else if ((ft_isalnum(str[i]) && is_operator(str[i + 1]))
			|| (is_operator(str[i]) && ft_isalpha(str[i + 1]))
			|| (str[i] == '|' && (str[i + 1] == '$' || str[i + 1] == '<'
					|| str[i + 1] == '>')))
			handle_operators(str, str_final, &i, &j);
		else
			str_final[j++] = str[i++];
	}
	str_final[j] = '\0';
	return (str_final);
}