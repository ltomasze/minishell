/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:59:40 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 16:01:57 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The function ft_tokenizer processes the input string inp, 
converting it into a list of tokens (t_token). It iteratively analyzes the input, 
identifying and creating tokens for redirections, pipes, 
and words using helper functions (ft_is_redir, ft_is_pipe, ft_is_word). 
In case of an error, it halts the execution and frees the allocated memory for the tokens. 
It skips spaces in the input and, if the analysis is successful, returns the list of tokens. 
Additionally, it frees th
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
The function ft_free_tokens frees the memory allocated for the list of tokens. 
It works by iterating through all the tokens, freeing the memory for their text, 
and then freeing the tokens themselves, until the list is exhausted. 
At the end, it sets the pointer to the start of the list to NULL 
to prevent access to already freed memory. 
The function first checks if the list is not empty before processing it.
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
The function create_token creates a new token with the given text str and type type, 
and then adds it to the end of the list of tokens pointed to by the tokens pointer. 
If the list is empty, the new token becomes its first element. 
The function returns 0 in case of success or -1 in case of errors (e.g., out of memory). 
The goal is to dynamically manage the list of tokens during lexical analysis.
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
