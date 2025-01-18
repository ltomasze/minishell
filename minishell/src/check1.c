/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:03:54 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/29 15:07:07 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

/* org_char = line[i] pobiera indeks w którym znalazł | lub >
i od tego miejsca sprawdza poprawność komendy
ft_isalnum - funkcja do znajdywania znaków w postaci liter i cyfr
line[i] == '>' && org_char != '>' aby unikną >>>*/

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

/* quote = line[*i] taki zapis oznacza że zmienna quote
 pobiera cudzosłów "czyli znak ze stringa" z pozycji line[i] ustalonej podczas działania
 funkcji check_for_missing_command;
 j = *i; ponieważ pobiera numer pozycji cudzysłowia anie znak cudzysłowia*/

static int	is_accepted_char(char ch)
{
	if (ch == 33 || ch == 61 || ch == 63
		|| (ch >= 35 && ch <= 47))
		return (1);
	else
		return (0);
}

/* !, =, ?, #, $, %, &, ', (, ), *, +, ,, -, ., / te znaki są ok */

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

int	check_for_missing_command(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if ((line[i] == '"' || line[i] == '\''))
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
