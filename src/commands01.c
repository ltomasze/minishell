/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands01.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:25:25 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:27:42 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_create_cmds(t_token *tokens, t_cmd *commands, int i);
static int	ft_free_args(int i, char **cmds);

/*
The `ft_count_tok` function counts the number of `T_WORD` tokens (words) in a token chain,  
stopping when it encounters a `T_PIPE` token.  
It works as follows: it starts with the first token, checks its type,  
and if it's not `T_WORD`, it skips two tokens ahead (skipping other token types).  
If it encounters a `T_WORD` token, it increments the counter,  
changes its type to `T_ARG`, and then moves to the next token.  
The function returns the count of `T_WORD` tokens before encountering a `T_PIPE`.
*/
static int	ft_count_tok(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type != T_WORD)
		{
			tokens = tokens->next->next;
			continue ;
		}
		i++;
		tokens->type = T_ARG;
		tokens = tokens->next;
	}
	return (i);
}

/*
The `ft_command` function analyzes the token pointed to by `*cur_token` and processes it as part of a command.  
If the token is of type `T_WORD`, it counts the subsequent related tokens (`ft_count_tok`),  
then creates a command in the current `t_cmd` structure using `ft_create_cmds`.  
In case of an error, it frees the memory for the tokens and commands and returns an error.  
If the token is of type `T_ARG`, it skips over all arguments, updating the `*cur_token` pointer.  
The function returns `0` when it finishes its work without errors.
*/
int	ft_command(t_token **cur_token, t_token *tokens,
t_cmd **cur_command, t_cmd *cmds)
{
	int	i;

	if (!(*cur_token))
		return (0);
	if ((*cur_token)->type == T_WORD)
	{
		i = ft_count_tok(*cur_token);
		if (ft_create_cmds(*cur_token, *cur_command, i) == 1)
		{
			ft_free_tokens(&tokens);
			ft_free_commands(&cmds);
			return (-1);
		}
	}
	if ((*cur_token)->type == T_ARG)
	{
		while (*cur_token && (*cur_token)->type == T_ARG)
			*cur_token = (*cur_token)->next;
	}
	return (0);
}
/*
The `ft_create_cmds` function creates an argument array 
for the command by processing tokens of type `T_ARG` 
until it encounters a `T_PIPE` token.  
For each `T_ARG` token, it creates a copy of the token's text 
and stores it in the `cmds` array.  
If an error occurs (e.g., failure to copy the token's text), 
it frees any previously allocated memory and ends the function.  
Finally, it assigns the argument array to the command structure (`commands->cmd`) 
and returns `0`, indicating success.
*/

static int	ft_create_cmds(t_token *tokens, t_cmd *commands, int i)
{
	char	**cmds;
	char	*arg;
	int		n;

	n = 0;
	cmds = malloc(sizeof(char *) * (i + 1));
	if (!cmds)
		return (ft_perror_message());
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type != T_ARG)
		{
			tokens = tokens->next;
			continue ;
		}
		arg = ft_strdup(tokens->text);
		if (!arg)
			return (ft_free_args(n, cmds));
		cmds[n++] = arg;
		tokens = tokens->next;
	}
	cmds[n] = NULL;
	commands->cmd = cmds;
	return (0);
}
/*
The `ft_free_args` function frees the memory allocated for the argument array `cmds`.  
It iterates over the elements of the array, freeing each of its positions, and then frees the array itself.  
At the beginning, it calls the `ft_perror_message` function, likely to display an error message.  
Finally, it returns `-1`, signaling an error.
*/

static int	ft_free_args(int i, char **cmds)
{
	int	j;

	ft_perror_message();
	j = 0;
	while (j < i)
		free(cmds[j++]);
	free(cmds);
	return (-1);
}
