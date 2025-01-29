/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 13:24:27 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 14:54:34 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `ft_tokenizer` przetwarza ciąg znaków `inp`, 
zamieniając go na listę tokenów (`t_token`). 
Iteracyjnie analizuje wejście, identyfikując i tworząc tokeny dla redirekcji, 
potoków oraz słów za pomocą funkcji pomocniczych (`ft_is_redir`, `ft_is_pipe`, 
`ft_is_word`). W przypadku błędu przerywa działanie i zwalnia pamięć tokenów. 
Pomija spacje w wejściu i, 
jeśli analiza przebiegnie poprawnie, zwraca listę tokenów. 
Dodatkowo zwalnia pamięć pierwotnej linii w `data->line`, 
aby uniknąć wycieków pamięci.
*/
t_token	*ft_tokenizer(t_data *data, char *inp)
{
	int		i;
	t_token	*tokens;

	tokens = NULL;
	i = 0;
	while (inp && inp[i])
	{
		if (ft_is_redir(inp, &i, &tokens) == -1)
			break ;
		if (ft_is_pipe(inp, &i, &tokens) == -1)
			break ;
		if (ft_is_word(inp, &i, &tokens, data) == -1)
			break ;
		if (inp[i] == ' ')
			i++;
	}
	if (inp && inp[i])
		ft_free_tokens(&tokens);
	free(data->line);
	data->line = NULL;
	return (tokens);
}

/*
Funkcja `ft_free_tokens` zwalnia pamięć przydzieloną dla listy tokenów.
Działa poprzez iterację po wszystkich tokenach, 
zwalniając pamięć dla ich tekstu, a następnie same tokeny, 
aż do wyczerpania listy. Na końcu ustawia wskaźnik na początek listy na `NULL`, 
aby zapobiec dostępowi do już zwolnionej pamięci. 
Funkcja najpierw sprawdza, czy lista nie jest pusta przed jej przetwarzaniem.
*/
void	ft_free_tokens(t_token **tokens)
{
	t_token	*temp;

	if (!tokens || !*tokens)
		return ;
	while (*tokens)
	{
		temp = *tokens;
		*tokens = (*tokens)->next;
		if (temp->text)
			free(temp->text);
		free(temp);
	}
	*tokens = NULL;
}

/*
Funkcja `create_token` tworzy nowy token z przekazanym tekstem `str` 
i typem `type`, 
a następnie dodaje go na koniec listy tokenów wskazywanej 
przez wskaźnik `tokens`. 
Jeśli lista jest pusta, nowy token staje się jej pierwszym elementem. 
Funkcja zwraca `0` w przypadku powodzenia 
lub `-1` w razie błędów (np. brak pamięci). 
Celem jest dynamiczne zarządzanie listą tokenów podczas analizy leksykalnej.
*/
int	create_token(char *str, int type, t_token **tokens)
{
	t_token	*new_token;
	t_token	*current;

	if (!str)
		return (-1);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (ft_perror_message());
	new_token->next = NULL;
	new_token->type = type;
	new_token->text = str;
	if (!*tokens)
		*tokens = new_token;
	else
	{
		current = *tokens;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (0);
}
