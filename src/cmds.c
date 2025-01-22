/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 12:46:20 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/22 13:39:37 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_redir(t_token **current_tok, t_token *head_tok,
		t_cmd **current_cmd, t_cmd *head_cmd)
{
	if (!*current_tok)
		return (0);
	if ((*current_tok)->type != T_WORD && (*current_tok)->type != T_PIPE)
	{
		if ((*current_tok)->next && (*current_tok)->next->type == T_WORD)
		{
			if (ft_set_redir(current_tok, *current_cmd) == -1)
			{
				ft_free_tokens(&head_tok);
				ft_free_commands(&head_cmd);
				return (-1);
			}
			(*current_tok) = (*current_tok)->next->next;
		}
		else
		{
			ft_free_tokens(&head_tok);
			ft_free_commands(&head_cmd);
			return (ft_error_message(REDIR_TO_OPR, -1));
		}
	}
	return (0);
}

t_cmd	*ft_commands(t_token *tokens)
{
	t_cmd	*commands;
	t_cmd	*current_cmd;
	t_token	*current_tok;

	commands = NULL;
	current_tok = tokens;
	if (ft_set_command(&commands) == -1)
	{
		ft_free_tokens(&tokens);
		return (NULL);
	}
	current_cmd = commands;
	while (current_tok)
	{
		if (ft_redir(&current_tok, tokens, &current_cmd, commands) == -1)
			return (NULL);
		/*if (ft_command(&current_tok, tokens, &current_cmd, commands) == -1)
			return (NULL);
		if (ft_pipe(&current_tok, tokens, &current_cmd, commands) == -1)
			return (NULL);*/
	}
	if (!tokens)
		return (NULL);
	ft_free_tokens(&tokens);
	return (commands);
}

/*ft_commands z tokenów tworzy listę poleceń do wykonania czyli t_cmd
*commands -wskaźnik na pierwszą listę poleceń, czyli wskaźnij na strukturę t_cmd
*current_cmd -wskaźnika na bieźące polecenie w strukturze.
*current_tok -wskaźnik na bieżacy token z listy tokenów.
ft_set_command funkcja która przydziela pamięć i inicjalizuje pola cmd infile
outfile które bedą póżniej wypełniane przez pętle current_tok itp.
*/

t_token	*ft_tokenizer(t_data *data, char *input)
{
	int		i;
	t_token	*tokens;

	tokens = NULL;
	i = 0;
	while (input && input[i])
	{
		if (ft_is_redir(input, &i, &tokens) == -1)
			break ;
		if (ft_is_pipe(input, &i, &tokens) == -1)
			break ;
		if (ft_is_word(input, &i, &tokens, data) == -1)
			break ;
		if (input[i] == ' ')
			i++;
	}
	if (input && input[i])
		ft_free_tokens(&tokens);
	free(data->line);
	data->line = NULL;
	return (tokens);
}

int	ft_cmds_creation(t_data *data)
{
	t_token	*tokens;

	tokens = ft_tokenizer(data, data->line);
	if (tokens == NULL)
	{
		data->cmd_exit_status = 1;
		return (-1);
	}
	if (ft_check_tokens(&tokens) == -1)
	{
		data->cmd_exit_status = 1;
		return (-1);
	}
	if (tokens == NULL)
		return (-1);
	data->cmd = ft_commands(tokens);
	if (data->cmd == NULL)
	{
		data->cmd_exit_status = 1;
		return (-1);
	}
	return (0);
}

/* ft_cmds_creation funkcja dzieli komendę na tokeny poprzez funkcję tokenizer
i sprawdza czy w przypadku pip lub redirection dane po każdej stronie od
tego operatora są prawidłowe, następnie tworzy strukturę komend pomiędzy
operatorami np. [echo "Hello, World!"] -> [grep "World"] -> [> output.txt]*/