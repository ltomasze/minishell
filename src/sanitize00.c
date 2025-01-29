/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize00.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:34:28 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:23:03 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_operator(char c);
static void	handle_operators(const char *str, char *str_final, int *i, int *j);
static void	handle_quotes(char *str, char *str_final, int *i, int *j);
static int	realloc_str_final(char **str_final,
				int j, int str_final_len);

/*
Funkcja `process_str` przetwarza wejściowy ciąg znaków `str` 
i zapisuje zaktualizowaną wersję w `str_final`. 
Iteruje przez `str`, pomijając zbędne spacje 
i tabulatory oraz obsługując specjalne przypadki, 
takie jak cudzysłowy (`handle_quotes`) 
i operatory (`handle_operators`). 
Jeśli bufor `str_final` staje się za mały, 
jego rozmiar jest zwiększany za pomocą `realloc_str_final`. 
Funkcja kończy przetwarzanie, 
dodając znak null (`\0`) na końcu ciągu i zwracając wynik. 
Jej celem jest normalizacja i interpretacja wejściowego 
ciągu znaków w kontekście składni poleceń.
*/
char	*process_str(char *str, char *str_final,
int str_final_len)
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
		else if (ft_isspace(str[i]) && (i == 0 || ft_isspace(str[i + 1])))
			i++;
		else if
			((ft_isalnum(str[i]) && is_operator(str[i + 1]))
			|| (str[i] == '|' && (str[i + 1] == '$'
			|| str[i + 1] == '<' || str[i + 1] == '>' )))
			handle_operators(str, str_final, &i, &j);
		else
			str_final[j++] = str[i++];
	}
	str_final[j] = '\0';
	return (str_final);
}

/*
Funkcja `is_operator` sprawdza, czy znak `c` 
jest jednym z operatorów (`>`, `<`, `|`). 
Jeśli tak, zwraca `1`; w przeciwnym razie zwraca `0`. 
Służy do rozpoznawania operatorów w analizowanym tekście.
*/
static int	is_operator(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	else
		return (0);
}

/*
Funkcja `handle_quotes` przetwarza ciągi ujęte w cudzysłowy w łańcuchu `str`. 
Ustala początek (`i_cp`) i koniec cudzysłowu, 
kopiując jego zawartość do `str_final`. 
Następnie aktualizuje pozycje wskaźników `i` i `j`. 
Jeśli znak po cudzysłowie jest operatorem, 
dodaje spację do `str_final`, aby oddzielić operator od reszty tekstu. 
Funkcja pomaga obsługiwać poprawne 
przetwarzanie cudzysłowów w wejściowych danych.
*/
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

/*
Funkcja `handle_operators` kopiuje operator z `str` do `str_final`, 
przesuwając wskaźniki `i` i `j`, 
a następnie dodaje spację po operatorze. 
Ułatwia to poprawne rozdzielanie 
operatorów w analizowanym ciągu znaków.
*/
static void	handle_operators(const char *str, char *str_final, int *i, int *j)
{
	str_final[(*j)++] = str[(*i)++];
	str_final[(*j)++] = ' ';
}

/*
Funkcja `realloc_str_final` zwiększa dwukrotnie 
rozmiar bufora `str_final`, 
aby pomieścić więcej danych. Tworzy nowy, większy bufor, 
kopiuje dotychczasową zawartość do nowego bufora, 
zwalnia stary bufor i aktualizuje wskaźnik. 
W przypadku błędu alokacji pamięci zwraca `-1`, 
a przy sukcesie zwraca nową długość bufora. 
Używana jest do dynamicznego zarządzania pamięcią dla rosnącego ciągu.
*/
static int	realloc_str_final(char **str_final,
int j, int str_final_len)
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
