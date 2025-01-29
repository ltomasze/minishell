/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:07:38 by mbany             #+#    #+#             */
/*   Updated: 2025/01/23 18:29:20 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `go_to_next_quote` znajduje pozycję 
następnego lub poprzedniego wystąpienia 
cudzysłowu w łańcuchu `line`, w zależności 
od wartości `go_back`. Jeśli `go_back` jest `false`,
 przeszukuje ciąg do przodu, a jeśli `true`, 
 przeszukuje do tyłu. Zapisuje nową pozycję 
 cudzysłowu w wskaźniku `i`. 
 Funkcja obsługuje identyfikację par 
 cudzysłowów w tekście.
*/
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

/*
Funkcja `ft_check_access` sprawdza dostęp do pliku
`file` w trybie odczytu (READ) lub zapisu (WRITE) 
za pomocą funkcji `access`. Jeśli plik nie istnieje
lub brak odpowiednich uprawnień, wypisuje
komunikat błędu i zwraca wartość ujemną. 
W przypadku sukcesu zwraca `0`. Używana jest 
do weryfikacji dostępności plików przed operacjami na nich.
*/
int	ft_check_access(char *file, int type)
{
	int	value;

	if (type == READ)
	{
		value = access(file, F_OK | R_OK);
		if (value < 0)
			return (ft_perror_message());
	}
	else if (type == WRITE)
	{
		value = access(file, F_OK | W_OK);
		if (value < 0)
			return (ft_perror_message());
	}
	return (0);
}
