/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 11:39:03 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/28 13:06:33 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_for_unclosed_quotes(char *line)
{
	int		i;
	bool	in_quote;
	char	quote;

	i = 0;
	in_quote = false;
	quote = '\0';
	while (line[i] != '\0')
	{
		if ((line[i] == '\'' || line[i] == '"'))
		{
			if (!in_quote)
			{
				quote = line[i];
				in_quote = true;
			}
			else if (line[i] == quote)
				in_quote = false;
		}
		i++;
	}
	if (in_quote == true)
		return (-1);
	return (0);
}

int	check_line_if_empty(char *line)
{
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, " \t");
	if (!trimmed_line)
	{
		perror("check_line_if_empty");
		return (1);
	}
	if (ft_strlen(trimmed_line) == 0)
	{
		free(trimmed_line);
		return (-1);
	}
	free(trimmed_line);
	return (0);
}

/*Wywoływana jest funkcja check_line_if_empty, która:
Usuwa białe znaki (spacje i tabulatory) z początku i końca linii przy pomocy ft_strtrim.
Sprawdza, czy wynikowa linia ma długość równą 0 (ft_strlen), 
czyli czy użytkownik nie wprowadził żadnych znaków poza białymi znakami.*/

int	check_syntax(char *line)
{
	if (check_line_if_empty(line))
		return (-1);
	if (check_for_unclosed_quotes(line))
		return (ft_error_message(MISS_QUOTE_ERR, -1));
	/*if (check_for_missing_command(line))
		return (ft_error_message(MISS_CMD_ERR, -1));*/
	return (0);
}