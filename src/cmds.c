/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 12:46:20 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/19 19:10:01 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


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
	/*data->cmd = ft_commands(tokens);*/
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