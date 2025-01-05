/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:06:03 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/05 13:44:54 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_skip_sq(int *n, char *str)
{
	(*n)++;
	while (str[*n] && str[*n] != '\'')
		(*n)++;
	(*n)++;
}

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