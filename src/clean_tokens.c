/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:18:15 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:19:28 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_rm_empty_tokens(t_token **tokens);
static void	ft_remove_token(t_token **head, t_token **to_del, t_token **prev);

/*
The `ft_check_tokens` function removes empty tokens  
from the token list using `ft_rm_empty_tokens`. If the operation fails  
(returning -1), it frees the entire token list using `ft_free_tokens`  
and returns -1. Otherwise, the function ends successfully, returning 0.  
Its purpose is to ensure that the token list is valid and free of empty elements.
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
The `ft_remove_empty_tokens` function removes empty tokens from the `tokens` list.  
It iterates through the token list, and if a token has an empty text (`text[0] == '\0'`),  
it checks whether its type matches a valid token sequence. If the sequence is incorrect,  
it returns an error with a message. If the token can be removed, it calls `ft_remove_token`  
to delete it and update the pointers, then continues the iteration.  
The function ensures the syntactic correctness of the token list and removes unnecessary elements.  
It returns `0` upon successful completion.
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
The `ft_remove_token` function removes the specified token `*to_del` from the token list,  
with `*head` as the head of the list. If the token to be removed is the first in the list,  
it updates the head pointer to the next element, frees the memory of the token's text and structure,  
and then sets `*to_del` to the new head. Otherwise, it updates the `next` pointer of the previous token (`*prev`)  
to skip the removed one, frees the memory of the removed token, and moves `*to_del` to the next element in the list.  
The function ensures proper memory management and the integrity of the token list structure.
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
