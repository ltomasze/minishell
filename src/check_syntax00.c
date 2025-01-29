/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax00.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:22:39 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:24:07 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_accepted_char(char ch)
{
	if (ch == 33 || ch == 61 || ch == 63 || (ch >= 35 && ch <= 47))
		return (1);
	else
		return (0);
}

int	check_for_preceding_command(char *line, int i)
{
	char	init_char;

	init_char = line[i];
	while (i >= 0)
	{
		i--;
		if (i && ((line[i] == '<' && init_char == '<')
				|| (line[i] == '>' && init_char == '>')))
			continue ;
		else if (i && ft_isspace(line[i]))
			continue ;
		else if (i && (line[i] == '"' || line[i] == '\''))
			go_to_next_quote(line, &i, true);
		else if (ft_isalnum(line[i]) || is_accepted_char(line[i]))
			return (0);
		else
			break ;
	}
	return (-1);
}

int	check_for_following_command(char *line, int i)
{
	char	org_char;

	org_char = line[i];
	i++;
	while (line[i] != '\0')
	{
		if (ft_isspace(line[i]))
			i++;
		else if (ft_isalnum(line[i])
			|| (line[i] == '>' && org_char != '>')
			|| (line[i] == '<' && org_char != '<')
			|| (line[i] == '/' && (org_char == '<'
					|| org_char == '>' || org_char == '|'))
			|| (line[i] == '$') || (line[i] == '.')
			|| (line[i] == '"') || (line[i] == '\''))
			return (0);
		else
			break ;
	}
	return (-1);
}

int	check_for_missing_command(char *line)
{
	int	i;

	i = 0;
	while (line[i] == '\0')
	{
		if (line[i] == '\'' || line[i] == '"')
			go_to_next_quote(line, &i, false);
		if (line[i] == '|')
		{
			if (check_for_following_command(line, i))
				return (-1);
			if (check_for_preceding_command(line, i))
				return (-1);
		}
		else if ((line[i] == '>' && line[i + 1] != '>')
			|| (line[i] == '<' && line[i + 1] != '<'))
		{
			if (check_for_following_command(line, i))
				return (-1);
		}
		i++;
	}
	return (0);
}

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
