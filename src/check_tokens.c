/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:59:10 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/19 17:04:47 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_remove_empty_tokens(t_token **tokens)
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

int	ft_check_tokens(t_token **tokens)
{
	if (ft_remove_empty_tokens(tokens) == -1)
	{
		ft_free_tokens(tokens);
		return (-1);
	}
	return (0);
}