/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:03:54 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/28 16:12:06 by ltomasze         ###   ########.fr       */
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

static int	is_accepted_char(char ch)
{
	if (ch == 33 || ch == 61 || ch == 63
		|| (ch >= 35 && ch <= 47))
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

/*Działanie funkcji check_for_missing_command
1. Przejście przez całą linię
Funkcja iteruje przez każdy znak linii wejściowej
za pomocą pętli while (line[i] != '\0').
Zmienna i jest wskaźnikiem na aktualną pozycję w ciągu znaków.

2. Obsługa cytatów (" lub ')
Jeżeli napotkano cudzysłów (" lub '):
Wywoływana jest funkcja go_to_next_quote, która przesuwa wskaźnik i
na koniec obecnego bloku cytowanego tekstu. 
Dzięki temu funkcja ignoruje zawartość cytatów podczas analizy składniowej.

3. Obsługa potoku (|)
Jeżeli napotkano operator potoku |:
Wywoływana jest funkcja check_for_following_command, 
aby sprawdzić, czy po operatorze potoku znajduje się poprawna komenda
lub argument.
Jeśli po | występuje błąd (np. brak komendy), funkcja zwraca -1.
Następnie wywoływana jest funkcja check_for_preceding_command,
aby upewnić się, że przed operatorem potoku znajduje się poprawna komenda
lub argument.
Jeśli przed | występuje błąd (np. brak komendy), funkcja również zwraca -1.

4. Obsługa operatorów redirekcji (> i <)
Jeżeli napotkano operator redirekcji:
Sprawdzane jest, czy operator redirekcji nie jest podwójny (np. >> lub <<) 
i czy występuje po nim poprawna komenda lub argument za pomocą 
check_for_following_command.
Jeżeli po operatorze redirekcji brak poprawnej komendy lub argumentu,
funkcja zwraca -1.

5. Kontynuacja pętli
Jeśli linia przeszła powyższe testy, wskaźnik i jest przesuwany
na następną pozycję, i analiza kontynuuje.

6. Wynik funkcji
Jeśli nie wykryto żadnych błędów składniowych, funkcja zwraca 0.
Jeśli wykryto błędy (np. brakująca komenda), funkcja zwraca -1.
*/