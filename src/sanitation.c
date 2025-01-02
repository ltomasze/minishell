/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:38:33 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/02 15:33:05 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	sanitaze_line(t_data *data)
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
		msg_error(SANITATION_ERR);
		return ;
	}
	data->line = process_str(str, data->line, str_len);
	data->line = optimize_str_final(data->line);
	if (!data->line)
	{
		free(str);
		msg_error(SANITATION_ERR);
		return ;
	}
	free(str);
}

/*potrzebujemy długości około dwukrotnie większej bo jęlsi np. > nie ma spacji
przed i za to jest to dodawane*/