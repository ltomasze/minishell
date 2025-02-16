/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:17:55 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:18:03 by ltomasze         ###   ########.fr       */
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
