/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:22:39 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:24:17 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_if_line_is_empty(char *line)
{
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, "\t");
	if (!trimmed_line)
	{
		perror("check_if_line_is_empty");
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

int	check_syntax(char *line)
{
	if (check_if_line_is_empty(line))
		return (-1);
	if (check_for_unclosed_quotes(line))
		return (ft_error_message(MISS_QUOTE_ERR, -1));
	if (check_for_missing_command(line))
		return (ft_error_message(MISS_CMD_ERR, -1));
	return (0);
}
