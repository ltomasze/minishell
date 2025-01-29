/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 15:07:32 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 16:48:39 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_valid_dollar(int *i, char *word, char **var);
static int	ft_expand_var(char *var, t_envp *env, char **word, int *i);
static int	ft_change_word(char *var, char **word, int *i, t_data *data);
static int	ft_exit_extension(char *var, char **word, int *i, t_data *data);

/*
Funkcja `ft_dollar` przetwarza wystąpienie zmiennej środowiskowej w słowie. 
Sprawdza poprawność zmiennej poprzez wywołanie `ft_valid_dollar` 
i uzyskuje jej wartość. 
Jeśli nie ma zmiennej do przetworzenia, zwraca `-1` i zwalnia pamięć. 
Gdy zmienna istnieje, indeks `i` jest odpowiednio aktualizowany. 
Następnie funkcja próbuje rozszerzyć zmienną środowiskową 
(funkcja `ft_expand_var`) i, 
jeśli uda się ją zmienić, wywołuje `ft_change_word` 
w celu zaktualizowania słowa. 
Jeśli którykolwiek z tych procesów zakończy się błędem 
(`value == -1`), funkcja zwraca `-1`. 
W przeciwnym razie, gdy wszystko przebiegnie poprawnie, zwraca `0`.
*/
int	ft_dollar(int *i, char **word, t_data *data)
{
	char	*var;
	int		value;

	value = ft_valid_dollar(i, *word, &var);
	if (value == -1)
	{
		free(word);
		return (-1);
	}
	else if (value >= 1)
		*i = *i + value;
	else
	{
		value = ft_expand_var(var, data->envp, word, i);
		if (value == 0)
		{
			value = ft_change_word(var, word, i, data);
			if (value == -1)
				return (-1);
		}
		else if (value == -1)
			return (-1);
	}
	return (0);
}

/*
Funkcja `ft_valid_dollar` sprawdza poprawność składni 
zmiennej środowiskowej po znaku `$` w podanym ciągu `word`. 
Obsługuje różne przypadki, takie jak `$$`, `$?`, 
nieprawidłowe znaki lub alfanumeryczne zmienne. 
Wartość `n` określa długość sekwencji. 
Gdy zmienna jest poprawna, alokuje pamięć dla `var`, kopiując nazwę zmiennej. 
Jeśli wystąpi błąd alokacji, zwraca -1, 
a w innych przypadkach zwraca długość przetworzonych znaków lub kod błędu.
*/
static int	ft_valid_dollar(int *i, char *word, char **var)
{
	int	n;

	n = 1;
	if (word[*i + n] && word[*i + n] == '$')
	{
		while (word[*i + n] && word[*i + n++] == '$')
			return (n);
	}
	if (word[*i + n] == '?')
		n++;
	else if (!(word[*i + n]) || (!ft_isalpha(word[*i + n])
			&& word[*i + n] != '_' ))
		return (1);
	else
	{
		while (word[*i + n] && (ft_isalnum(word[*i + n])
				|| word[*i + n] == '_'))
			n++;
	}
	*var = malloc(sizeof(char) * n);
	if (!(*var))
		return (ft_perror_message());
	ft_strlcpy(*var, &word[1 + (*i)], n);
	return (0);
}

/*
Funkcja `ft_expand_var` zamienia zmienną środowiskową 
w podanym ciągu `word` (rozpoczynającą się od `$`) 
na odpowiadającą jej wartość z listy środowiskowej `env`. 
Najpierw znajduje zmienną w liście, a następnie alokuje nowy ciąg, 
zastępując nazwę zmiennej jej wartością. 
Jeśli zmienna nie istnieje lub nie ma wartości, zwraca 0. 
W przypadku błędów alokacji zwraca -1. 
Funkcja aktualizuje wskaźnik na przetworzony ciąg 
i odpowiednio przesuwa indeks `i`.
*/
static int	ft_expand_var(char *var, t_envp *env, char **word, int *i)
{
	t_envp	*env_var;
	char	*str;
	int		n;
	int		value_len;

	n = ft_strlen(var);
	env_var = fetch_envp_node(env, var);
	if (!env_var || !env_var->value[n + 1])
		return (0);
	free(var);
	value_len = ft_strlen(&env_var->value[n + 1]);
	str = malloc(sizeof(char) * (ft_strlen(*word) - n + value_len));
	if (!str)
	{
		free(word);
		return (ft_perror_message());
	}
	ft_strlcpy(str, *word, *i + 1);
	ft_strlcpy(&str[*i], &env_var->value[n + 1], value_len + 1);
	ft_strlcpy(&str[*i + value_len], &(*word)[*i + n + 1],
		ft_strlen(*word) - *i - n + 1);
	free(*word);
	*word = str;
	*i = *i + value_len -1;
	return (1);
}

/*
Funkcja `ft_change_word` usuwa zmienną środowiskową 
z ciągu `word` zaczynającą się od `$`, 
zastępując ją pustym tekstem, chyba że zmienna to `$?`, 
która jest obsługiwana osobno przez `ft_exit_extension`. 
Oblicza długości zmiennej i oryginalnego ciągu, alokuje nowy ciąg bez zmiennej, 
kopiuje odpowiednie fragmenty tekstu, aktualizuje wskaźnik `word`, 
a następnie zwalnia starą pamięć. W przypadku błędów alokacji zwraca -1.
*/
static int	ft_change_word(char *var, char **word, int *i, t_data *data)
{
	char	*new_word;
	int		word_len;
	int		v_len;

	if (var[0] == '?')
	{
		if (ft_exit_extension(var, word, i, data) == -1)
			return (-1);
		return (0);
	}
	v_len = ft_strlen(var);
	word_len = ft_strlen(*word);
	new_word = malloc(sizeof(char) * (word_len - v_len + 1));
	if (!new_word)
	{
		free(var);
		free(*word);
		return (ft_perror_message());
	}
	ft_strlcpy(new_word, *word, *i + 1);
	ft_strlcpy(&new_word[*i], &(*word)[*i + v_len + 1], word_len - *i - v_len);
	free(var);
	free(*word);
	*word = new_word;
	return (0);
}

/*
Funkcja `ft_exit_extension` zastępuje w ciągu `word` zmienną `$?` 
wartością kodu wyjścia ostatniego polecenia,
 przechowywaną w `data->cmd_exit_status`. 
Najpierw zwalnia pamięć dla zmiennej `var`, 
następnie konwertuje kod wyjścia na ciąg znaków za pomocą `ft_itoa`. 
Alokuje nowy ciąg, w którym `$?` zastępowane jest tym kodem, 
aktualizuje wskaźnik `word` i przesuwa indeks `i`. 
Jeśli wystąpi błąd alokacji pamięci, funkcja zwalnia zasoby i zwraca -1.
*/
static int	ft_exit_extension(char *var, char **word, int *i, t_data *data)
{
	char	*new_word;
	char	*exit_code;
	int		exit_len;
	int		word_len;

	free(var);
	exit_code = ft_itoa(data->cmd_exit_status);
	if (!exit_code)
		return (ft_perror_free(NULL, *word, NULL));
	exit_len = ft_strlen(exit_code);
	word_len = ft_strlen(*word);
	new_word = malloc(sizeof(char) * (word_len + exit_len - 1));
	if (!new_word)
		return (ft_perror_free(NULL, *word, exit_code));
	ft_strlcpy(new_word, *word, *i + 1);
	ft_strlcpy(&new_word[*i], exit_code, exit_len + 1);
	ft_strlcpy(&new_word[*i + exit_len], &(*word)[*i + 2], word_len - *i - 1);
	free(*word);
	free(exit_code);
	*word = new_word;
	*i = *i + exit_len - 1;
	return (0);
}
