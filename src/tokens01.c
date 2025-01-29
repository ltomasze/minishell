/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 15:09:41 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:36:35 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `ft_create_word_tok` tworzy token reprezentujący 
słowo w ciągu wejściowym `str`, 
zaczynając od pozycji `*i`. 
Oblicza długość słowa `n`, 
uwzględniając sekwencje znaków wewnątrz pojedynczych (`'`) 
i podwójnych (`"`) cudzysłowów, oraz ignoruje spacje jako separatory. 
Gdy długość słowa zostanie wyznaczona, wyodrębnia jego treść 
i tworzy odpowiedni token za pomocą `ft_extract_word`. 
Po zakończeniu aktualizuje wartość `*i`, 
przesuwając wskaźnik na koniec przetworzonego słowa. 
Funkcja zwraca `0` w przypadku sukcesu lub `-1`, 
gdy wystąpi błąd. Celem jest poprawne przetwarzanie 
słów w analizatorze leksykalnym.
*/

int	ft_create_word_tok(char *str, int *i, t_token **tokens, t_data *data)
{
	int	n;

	n = 0;
	while (str[*i + n] && str[*i + n] != ' ')
	{
		if (str[*i + n] == '\'')
			ft_skip_sq(&n, &str[*i]);
		else if (str[*i + n] == '\"')
		{
			n++;
			while (str[*i + n] && str[*i + n] != '\"')
				n++;
			n++;
		}
		else
		{
			while (str[*i + n] && !ft_strchr("\"\' ", str[*i + n]))
				n++;
		}
	}
	if (ft_extract_word(&str[*i], &n, tokens, data) == -1)
		return (-1);
	*i = *i + n;
	return (0);
}

/*
Funkcja `ft_extract_word` tworzy nowy token typu słowo 
(T_WORD) na podstawie ciągu znaków `str` o długości `*n`, 
alokując pamięć na nowy ciąg `word` i kopiując do niego odpowiedni fragment. 
Następnie sprawdza obecność symbolu `$` za pomocą `ft_check_for_dollar`, 
a później przetwarza słowo funkcją `ft_cross_word`. 
Jeśli żadna z tych operacji nie zwróci błędu, 
tworzy nowy token i dodaje go do listy `tokens`. 
Funkcja umożliwia rozdzielanie słów na tokeny przy analizie leksykalnej, 
z uwzględnieniem zmiennych środowiskowych i specjalnych modyfikacji.
*/
int	ft_extract_word(char *str, int *n, t_token **tokens, t_data *data)
{
	char	*word;
	int		value;

	word = malloc(sizeof(char) * (*n + 1));
	if (!word)
		return (ft_perror_message());
	ft_strlcpy(word, str, (*n) + 1);
	if (ft_check_for_dollar(&word, data) == -1)
		return (-1);
	value = ft_cross_word(&word, tokens);
	if (value == -1)
	{
		free(word);
		return (-1);
	}
	else if (value == 0)
		return (0);
	if (create_token(word, T_WORD, tokens) == -1)
	{
		free(word);
		return (-1);
	}
	return (0);
}

/*
Funkcja `ft_check_for_dollar` analizuje zawartość ciągu `*word` 
w poszukiwaniu znaków specjalnych: pojedynczych apostrofów (`'`), 
znaku dolara (`$`), oraz podwójnych cudzysłowów (`"`). 
W przypadku znalezienia: pomija zawartość w apostrofach, 
przetwarza zmienne środowiskowe za pomocą `ft_dollar`, 
oraz interpretuje zawartość w cudzysłowach przez `ft_cross_dq`. 
Funkcja modyfikuje zawartość `*word` zgodnie z kontekstem, 
zwracając -1 w razie błędu. Jej celem jest przetwarzanie 
i rozwiązywanie zmiennych środowiskowych 
i specjalnych sekwencji w analizowanym tekście.
*/
int	ft_check_for_dollar(char **word, t_data *data)
{
	int	i;

	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '\'')
			ft_skip_sq(&i, *word);
		else if ((*word)[i] == '$')
		{
			if (ft_dollar(&i, word, data) == -1)
				return (-1);
		}
		else if ((*word)[i] == '\"')
		{
			if (ft_cross_dq (&i, word, data) == -1)
				return (-1);
		}
		else
			i++;
	}
	return (0);
}

/*
Funkcja `ft_cross_word` przetwarza słowo, iterując po jego znakach. 
Jeśli natrafi na cudzysłów (pojedynczy lub podwójny), 
wywołuje funkcję `ft_clear_quote`, aby usunąć znaki cytatu. 
Jeśli napotka białe znaki (spacja, tabulator, itp.), 
wywołuje `ft_cut_token`, aby podzielić słowo na tokeny; 
jeśli zwróci błąd lub zakończy proces, odpowiednio przerywa działanie. 
W przypadku innych znaków po prostu przechodzi do następnego. 
Funkcja kończy się zwróceniem `1`, jeśli przetworzenie się powiodło.
*/
int	ft_cross_word(char **word, t_token **tokens)
{
	int	i;
	int	value;

	i = 0;
	while ((*word)[i])
	{
		if ((*word)[i] == '\'' || (*word)[i] == '\"')
		{
			if (ft_clear_quote(&i, word, (*word)[i]) == -1)
				return (-1);
		}
		else if (ft_strchr(" \t\r\n\v\f", (*word)[i]))
		{
			value = ft_cut_token(&i, word, tokens);
			if (value == -1)
				return (-1);
			if (value == 0)
				return (0);
		}
		else
			i++;
	}
	return (1);
}

/*
Funkcja `ft_cut_token` przetwarza słowo, 
tworząc jego token. 
Zaczyna od alokacji pamięci na nowy ciąg znaków `tok_str` 
i kopiuje część `*word` do tego ciągu. Następnie wywołuje `create_token`, 
by utworzyć token typu `T_WORD`. 
Jeśli to się nie uda, zwraca błąd i zwalnia pamięć. 
Następnie funkcja ignoruje białe znaki, 
aż napotka kolejny token lub koniec słowa. 
Jeśli nie ma więcej znaków, zwalnia pamięć i kończy. 
Jeśli napotka kolejne znaki, 
alokuje nową pamięć dla reszty słowa, kopiuje ją do `new_word`, 
aktualizuje `*word`, 
a indeks `i` ustawia na 0, by przetwarzać nowe słowo. Funkcja zwraca `1`, 
gdy słowo jest kontynuowane, lub `0`, gdy nie ma nic więcej do przetworzenia.
*/
int	ft_cut_token(int *i, char **word, t_token **tokens)
{
	char	*tok_str;
	char	*new_word;

	tok_str = malloc(sizeof(char) * (*i + 1));
	if (!tok_str)
		return (ft_perror_message());
	ft_strlcpy(tok_str, *word, *i + 1);
	if (create_token(tok_str, T_WORD, tokens) == -1)
		return (ft_perror_free(tok_str, NULL, NULL));
	while ((*word)[*i] && ft_strchr(" \t\r\n\v\f", (*word)[*i]))
		(*i)++;
	if (!(*word)[*i])
	{
		free(*word);
		return (0);
	}
	new_word = malloc(sizeof(char) * (ft_strlen(&(*word)[*i]) + 1));
	if (!new_word)
		return (ft_perror_message());
	ft_strlcpy(new_word, &(*word)[*i], (ft_strlen(&(*word)[*i]) + 1));
	free(*word);
	*word = new_word;
	*i = 0;
	return (1);
}
