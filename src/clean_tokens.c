/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_toknes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 13:46:10 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 16:38:21 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_rm_empty_tokens(t_token **tokens);
static void	ft_remove_token(t_token **head, t_token **to_del, t_token **prev);

/*
Funkcja `ft_check_tokens` usuwa puste tokeny 
z listy tokenów za pomocą `ft_rm_empty_tokens`, 
a jeśli operacja się nie powiedzie (zwraca -1), 
zwalnia całą listę tokenów funkcją `ft_free_tokens` i zwraca -1. 
W przeciwnym razie funkcja kończy działanie sukcesem, zwracając 0. 
Ma na celu upewnienie się, 
że lista tokenów jest poprawna i wolna od pustych elementów.
*/
int	ft_check_tokens(t_token **tokens)
{
	if (ft_rm_empty_tokens(tokens) == -1)
	{
		ft_free_tokens(tokens);
		return (-1);
	}
	return (0);
}

/*
Funkcja `ft_remove_empty_tokens` usuwa puste tokeny z listy `tokens`. 
Iteruje przez listę tokenów, a jeśli token ma pusty tekst (`text[0] == '\0'`), 
sprawdza, czy jego typ jest zgodny z poprawną sekwencją tokenów; 
w przypadku błędnej sekwencji zwraca błąd z komunikatem. 
Jeśli token można usunąć, wywołuje `ft_remove_token`, 
aby go usunąć i zaktualizować wskaźniki, po czym kontynuuje iterację. 
Funkcja gwarantuje poprawność składni listy tokenów i usuwa zbędne elementy. 
Na końcu zwraca `0` przy poprawnym zakończeniu.
*/
int	ft_rm_empty_tokens(t_token **tokens)
{
	t_token	*curr;
	t_token	*prev;

	prev = NULL;
	curr = *tokens;
	while (curr)
	{
		if (curr->text && !curr->text[0])
		{
			if (prev && (prev->type != T_WORD && prev->type != T_PIPE
					&& prev->type != T_HEREDOC))
				return (ft_error_message(NULL_REDIR, -1));
			if ((prev && prev->type == T_WORD) || !prev)
			{
				ft_remove_token(tokens, &curr, &prev);
				continue ;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

/*
Funkcja `ft_remove_token` usuwa wskazany token `*to_del` z listy tokenów, 
której głową jest `*head`. Jeśli usuwany token jest pierwszym w liście, 
aktualizuje wskaźnik głowy na następny element, 
zwalnia pamięć tekstu i struktury tokenu, 
a następnie ustawia `*to_del` na nową głowę. W przeciwnym razie,
aktualizuje wskaźnik `next` poprzedniego tokenu (`*prev`) tak, 
by omijał usuwany, zwalnia pamięć usuwanego tokenu, 
i przestawia `*to_del` na kolejny element listy. 
Funkcja zapewnia poprawne zarządzanie pamięcią i strukturą listy tokenów.
*/
void	ft_remove_token(t_token **head, t_token **to_del, t_token **prev)
{
	if (*head == *to_del)
	{
		*head = (*to_del)->next;
		free((*to_del)->text);
		free(*to_del);
		*to_del = *head;
		return ;
	}
	(*prev)->next = (*to_del)->next;
	free((*to_del)->text);
	free(*to_del);
	*to_del = (*prev)->next;
}
