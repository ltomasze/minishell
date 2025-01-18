/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:54:04 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/18 17:47:03 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
/*funkcja do używania zmiennej $? czyli status zakończenia
np "echo $?" data->cmd_exit_status = 0
(status zakończenia ostatniego polecenia to 0)*/
/*
Linijka 1:
free(var);
Funkcja zaczyna od zwolnienia pamięci zajmowanej przez wskaźnik var.
W tym przypadku var to ciąg znaków, który zawiera wartość zmiennej $?.
Zmienna ta jest już zbędna po wykonaniu operacji.

Linijka 2:
exit_code = ft_itoa(data->cmd_exit_status);
Funkcja ft_itoa konwertuje data->cmd_exit_status (status zakończenia polecenia,
np. 0 lub 1) na ciąg znaków. Jeśli data->cmd_exit_status = 0,
wynik exit_code to "0". Funkcja ft_itoa
zwraca dynamicznie alokowany ciąg znaków reprezentujący liczbę.
Przykład:
data->cmd_exit_status = 0 → exit_code = "0"

Linijka 4:
exit_len = ft_strlen(exit_code);
Obliczamy długość ciągu exit_code (liczba cyfr w liczbie).
Dla exit_code = "0" długość to 1.

Linijka 5:
word_len = ft_strlen(*word);
Obliczamy długość ciągu *word, który zawiera pełny ciąg do przetworzenia,
np. "echo $?". Długość tego ciągu to 8.
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
/*funkcja do zmiany zmiennej $var jak $USER na np. John*/
/*
var: Zawiera nazwę zmiennej środowiskowej do usunięcia (np. "USER" w przypadku $USER).
word: Wskaźnik do ciągu znaków zawierającego analizowany tekst (np. "echo $USER").
i: Indeks w word, gdzie znajduje się $.
data: Struktura z dodatkowymi danymi, np. zmiennymi środowiskowymi.
*/
/*
if (var[0] == '?')
{
	if (ft_exit_extension(var, word, i, data) == -1)
		return (-1);
	return (0);
}
Jeśli var to "?", funkcja wywołuje ft_exit_extension, która obsługuje przypadek $? (status ostatniego polecenia).
Jeśli ft_exit_extension zwróci -1, funkcja kończy się błędem.
W przeciwnym wypadku zwraca 0, co oznacza poprawne zakończenie dla przypadku $?.
*/
/*
v_len = ft_strlen(var);
word_len = ft_strlen(*word);
v_len: Długość zmiennej var (np. "USER" → v_len = 4).
word_len: Długość całego ciągu word (np. "echo $USER" → word_len = 10).
*/
/*
new_word = malloc(sizeof(char) * (word_len - v_len + 1));
Alokuje pamięć na nowy ciąg new_word, który będzie kopią word, ale bez $var.
Nowa długość: word_len - v_len + 1 (np. "echo $USER" → "echo " ma długość 6).
*/
/*
Kopiowanie pierwszej części ciągu (przed $)
ft_strlcpy(new_word, *word, *i + 1);
Wynik: Jeśli word = "echo $USER" i *i = 5, new_word zawiera "echo ".
Kopiowanie części po zmiennej
ft_strlcpy(&new_word[*i], &(*word)[*i + v_len + 1], word_len - *i - v_len);
Dla word = "echo $USER", *i = 5, v_len = 4:
Kopiuje część od pozycji *i + v_len + 1 = 10 (po $USER).
Wynik: new_word = "echo ".
Przypisanie wskaźnika do nowego ciągu
*word = new_word;
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

/*funkcja do zamiany zmiennej środowiskowej określonej jako var na jej wartość
z listy zmiennych środowiskowych env, działa na tekscie przechowanym w *word i
odpowiednio go modyfikuje*/
/*
char *var: Zawiera nazwę zmiennej środowiskowej, np. "USER".
t_envp *env: Lista zmiennych środowiskowych w strukturze t_envp, gdzie każda zmienna ma formę key=value.
char **word: Wskaźnik na ciąg znaków, który zawiera $var do zamiany.
int *i: Wskaźnik na bieżący indeks w ciągu *word, wskazujący pozycję znaku $.
*/
/*
fetch = pobierać
Funkcja fetch_envp_node przeszukuje env w poszukiwaniu klucza o nazwie var.
Jeśli znajdzie pasującą zmienną, zwraca wskaźnik do jej wartości (env_var->value).
*/
/*
value_len = ft_strlen(&env_var->value[n + 1]);
env_var->value zawiera pełny wpis key=value.
Aby dostać się do wartości, omija nazwę zmiennej i znak = (przesunięcie n + 1).
Np. jeśli env_var->value = "USER=John", &env_var->value[n + 1]
wskazuje na "John", a value_len = 4.
*/
/*
Kopiowanie tekstu przed $var:
ft_strlcpy(str, *word, *i + 1);
kopiuje część oryginalnego tekstu z *word,
znajdującą się przed indeksem *i (znakiem $), do str.
ft_strlcpy(&str[*i], &env_var->value[n + 1], value_len + 1);
Kopiuje wartość zmiennej środowiskowej (np. "John") do nowego ciągu str,
zaczynając od pozycji *i (tam, gdzie był $).
Kopiowanie reszty tekstu po $var:
ft_strlcpy(&str[*i + value_len], 
&(*word)[*i + n + 1], ft_strlen(*word) - *i - n + 1);
*/
/*
*i = *i + value_len - 1;
Indeks *i przesuwa się na koniec wstawionej wartości zmiennej,
aby kontynuować przetwarzanie w następnych iteracjach.
*/

static int	ft_valid_dollar(int *i, char *word, char **var)
{
	int	n;

	n = 1;
	if (word[*i + n] && word[*i + n] == '$')
	{
		while (word[*i + n] && word[*i + n++] == '$')
			;
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
i: Wskaźnik na bieżący indeks w analizowanym ciągu znaków.
word: Analizowany ciąg tekstowy, w którym znajduje się znak $.
var: Wskaźnik na wskaźnik, gdzie zostanie zapisany ciąg reprezentujący nazwę zmiennej środowiskowej, jeśli jest poprawna.
Zwracana wartość: Liczba znaków przetworzonych przez funkcję (lub błąd, jeśli funkcja nie powiodła się).
*/
/*
else if (!(word[*i + n]) || (!ft_isalpha(word[*i + n]) && word[*i + n] != '_'))
    return (1);
Jeśli:
Po $ nie ma już żadnego znaku (koniec ciągu) lub
Po $ znajduje się niealfabetyczny znak (np. @, #, 1) i nie jest to _ (podkreślenie),
Funkcja zwraca 1, sygnalizując, że $ nie jest poprawną zmienną.
Przykład:
word = "$@HOME":
$@ nie jest poprawną zmienną, funkcja zwraca 1.
*/
/*
else
	{
		while (word[*i + n] && (ft_isalnum(word[*i + n])
				|| word[*i + n] == '_'))
			n++;
	}
tutaj funkcja sprawdza czy znaki to A-Z, a-z, 0-9, jęsli tak to uznaje że jest ok
*/
/*
n = 1 bo funkcja sprawdza po znalezieniu pierwszego dolara;
$USER: Nazwa zmiennej środowiskowej.
$$: PID procesu.
$?: Status wyjścia ostatniego polecenia jak 0 to sukces, 1 to błąd.*/

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
int *i: wskaźnik na bieżącą pozycję w analizowanym ciągu znaków.
char **word: wskaźnik do przetwarzanego ciągu tekstowego zawierającego dolary.
t_data *data: struktura zawierająca dane, w tym informacje o zmiennych środowiskowych (data->envp).
*/
/*
else if (value >= 1)
		*i = *i + value;
gdy jest przypadek: $$, $$$ i wiecej $ lub $?
*/