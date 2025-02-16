/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize01.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:34:28 by mbany             #+#    #+#             */
/*   Updated: 2025/02/16 15:52:49 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The optimize_str_final function creates a new copy of the string str_final using ft_strdup,
frees the memory occupied by the original string,
and returns a pointer to the new copy.
It is used for memory management to replace the original pointer with a new,
independent string.
*/
static char	*optimize_str_final(char *str_final)
{
	char	*new_str;

	if (!str_final)
		return (NULL);
	new_str = ft_strdup(str_final);
	free(str_final);
	if (!new_str)
		return (NULL);
	return (new_str);
}

/*
The sanitize_line function cleans the entered line of text in the data structure by
removing leading and trailing spaces and tabs using ft_strtrim,
dynamically allocates memory for an updated string with double the length,
processes the string using process_str,
and then optimizes it with optimize_str_final.
In case of errors (e.g., memory allocation issues), it displays an error message and frees the allocated memory.
The function is used to prepare the entered text for further processing.
*/
void	sanitize_line(t_data *data)
{
	char	*str;
	int		str_len;

	str = ft_strtrim(data->line, " \t");
	free(data->line);
	if (!str)
		return ;
	str_len = ft_strlen(str) * 2;
	data->line = malloc(sizeof(char) * (ft_strlen(str) * 2));
	if (!data->line)
	{
		free(str);
		msg_error(SANITIZE_ERROR);
		return ;
	}
	data->line = process_str(str, data->line, str_len);
	data->line = optimize_str_final(data->line);
	if (!data->line)
	{
		free(str);
		msg_error(SANITIZE_ERROR);
		return ;
	}
	free(str);
}
