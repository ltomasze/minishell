/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens00.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 15:09:41 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:37:01 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_single_redirection(char x, t_token **tokens, char *str);

/*
Funkcja `ft_append_redir` sprawdza, 
czy znak w `input[*i]` to znak `<` lub `>`, 
a następnie przypisuje do zmiennej `str` odpowiednią wartość `"<<"` 
lub `">>"`. Jeśli alokacja pamięci dla `str` nie powiedzie się, zwraca błąd. 
Funkcja tworzy token dla typu `T_HEREDOC` 
lub `T_APPEND` za pomocą funkcji `create_token`, 
a następnie przesuwa indeks `*i` o 2. 
Jeśli wystąpi błąd podczas tworzenia tokenu, zwraca `-1`. 
Celem funkcji jest obsługa różnych rodzajów redirekcji (`<<` lub `>>`).
*/
static int	ft_append_redir(char *input, int *i, t_token **tokens, char *str)
{
	int	error;

	if (input [*i] == '<')
		str = ft_strdup("<<");
	else
		str = ft_strdup(">>");
	if (!str)
		return (ft_perror_message());
	if (input[*i] == '<')
		error = create_token(str, T_HEREDOC, tokens);
	else
		error = create_token(str, T_APPEND, tokens);
	if (error == -1)
	{
		free(str);
		return (-1);
	}
	*i = *i + 2;
	return (0);
}

/*
Funkcja `ft_is_redir` sprawdza, 
czy znak w łańcuchu `input` pod indeksem `*i` to znak redirekcji (`<` lub `>`). 
W przypadku podwójnego znaku redirekcji 
(`<<` lub `>>`) wywołuje `ft_append_redir`, tworząc odpowiedni token. 
W przypadku pojedynczej redirekcji tworzy token 
za pomocą `ft_single_redirection` 
i przesuwa indeks `*i`. W razie błędu zwraca `-1`, a w przeciwnym razie `0`, 
pozwalając kontynuować analizę tokenów. Funkcja identyfikuje redirekcje 
i dodaje je do listy tokenów.
*/
int	ft_is_redir(char *input, int *i, t_token **tokens)
{
	int		error;
	char	*str;

	str = NULL;
	if (ft_strchr("<>", input[*i]))
	{
		if (input[*i] == input [*i + 1])
		{
			if (ft_append_redir(input, i, tokens, str) == -1)
				return (-1);
			return (0);
		}
		error = ft_single_redirection(input [*i], tokens, str);
		if (error != 0)
			return (-1);
		(*i)++;
		return (0);
	}
	return (0);
}

/*
Funkcja `ft_is_pipe` sprawdza, 
czy w danym miejscu ciągu wejściowego `input` 
(indeks wskazany przez `*i`) znajduje się znak `|`. 
Jeśli tak, kopiuje ten znak do nowo utworzonego ciągu `str` 
za pomocą `ft_strdup`, a następnie tworzy nowy token typu `T_PIPE`, 
przekazując go do listy tokenów za pomocą `create_token`. 
W przypadku błędów (np. brak pamięci podczas tworzenia `str` 
lub niepowodzenie w `create_token`), 
funkcja obsługuje błąd przez zwrócenie odpowiedniej wartości (`-1`). 
Gdy token zostanie utworzony pomyślnie, 
funkcja zwiększa indeks `*i` (pomija przetworzony znak w `input`) 
i zwraca `0`. Funkcja działa, 
by identyfikować znaki `|` jako operator rurociągu w procesie tokenizacji.
*/
int	ft_is_pipe(char *input, int *i, t_token **tokens)
{
	int		error;
	char	*str;

	if (input[*i] == '|')
	{
		str = ft_strdup("|");
		if (!str)
			return (ft_perror_message());
		error = create_token(str, T_PIPE, tokens);
		if (error == -1)
		{
			free(str);
			return (-1);
		}
		(*i)++;
	}
	return (0);
}

/*
Funkcja `ft_is_word` sprawdza, 
czy znak w ciągu wejściowym `input` na pozycji `*i` 
nie jest jednym ze znaków specjalnych `|`, `<`, `>` lub spacji. 
Jeśli warunek jest spełniony, wywołuje funkcję `ft_create_word_tok`, 
która tworzy token typu "słowo" i dodaje go do listy tokenów. 
W przypadku błędu w `ft_create_word_tok` funkcja zwalnia pamięć 
przydzieloną dla `input` i zwraca `-1`, aby zasygnalizować niepowodzenie. 
W przeciwnym razie funkcja zwraca `0`. Ma na celu identyfikację 
i tokenizację zwykłych słów w procesie analizy leksykalnej wejścia.*/
int	ft_is_word(char *input, int *i, t_token **tokens, t_data *data)
{
	if (!(ft_strchr("| <>", input[*i])))
	{
		if (ft_create_word_tok(input, i, tokens, data) == -1)
		{
			free(input);
			return (-1);
		}
	}
	return (0);
}

/*
Funkcja `ft_single_redirection` obsługuje pojedyncze znaki 
przekierowań wejścia (`<`) lub wyjścia (`>`), tworząc odpowiedni token. 
Na podstawie znaku `x` przypisuje odpowiedni typ 
(`T_IN_REDIR` lub `T_OUT_REDIR`) 
oraz kopiuje znak do nowego łańcucha za pomocą `ft_strdup`. 
Następnie wywołuje `create_token`, 
aby dodać nowy token do listy `tokens`. 
Funkcja zwraca `0` w przypadku powodzenia lub `-1`, 
jeśli wystąpi błąd (np. brak pamięci). 
Jej celem jest poprawne rozpoznawanie 
i dodawanie tokenów dla operatorów przekierowań w analizatorze leksykalnym.
*/
static int	ft_single_redirection(char x, t_token **tokens, char *str)
{
	int	type;
	int	error;

	if (x == '<')
	{
		type = T_IN_REDIR;
		str = ft_strdup("<");
	}
	else
	{
		type = T_OUT_REDIR;
		str = ft_strdup(">");
	}
	if (!str)
		return (ft_perror_message());
	error = create_token(str, type, tokens);
	if (error == -1)
	{
		free(str);
		return (-1);
	}
	return (0);
}
