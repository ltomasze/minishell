/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands00.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:22:33 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:24:42 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The `ft_commands_creation` function is responsible for creating the command structure  
based on the input string in `data->line`. It tokenizes the line using `ft_tokenizer`,  
checks the validity of the tokens with `ft_check_tokens`, and then generates the command structure  
with `ft_commands`. If an error occurs at any stage (tokenization, verification, or command creation),  
the function sets the exit status `cmd_exit_status` to `1` and returns `-1`.  
On success, it returns `0`.
*/
int	ft_commands_creation(t_data *data)
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

/*
The `ft_commands` function transforms the token list into a list of command structures.  
It creates the initial element of the command list and then iterates through the tokens,  
handling redirections, command arguments, and pipeline operators using helper functions.  
If an error occurs during processing, the function stops and returns `NULL`.  
After completing the process, the tokens are freed, and the command list is returned.  
This function structures the input for further command processing.
*/
t_cmd	*ft_commands(t_token *tokens)
{
	t_cmd	*commands;
	t_cmd	*current_cmd;
	t_token	*current_token;

	commands = NULL;
	current_token = tokens;
	if (ft_set_command(&commands) == -1)
	{
		ft_free_tokens(&tokens);
		return (NULL);
	}
	current_cmd = commands;
	while (current_token)
	{
		if (ft_redir(&current_token, tokens, &current_cmd, commands) == -1)
			return (NULL);
		if (ft_command(&current_token, tokens, &current_cmd, commands) == -1)
			return (NULL);
		if (ft_pipe(&current_token, tokens, &current_cmd, commands) == -1)
			return (NULL);
	}
	if (!tokens)
		return (NULL);
	ft_free_tokens(&tokens);
	return (commands);
}
/*
The `ft_set_command` function creates a new command structure (`t_cmd`),  
initializes its fields with default values, and adds it to the end of the `commands` list.  
If the command list is empty, the new structure becomes its first element.  
This function is used for dynamically building the command list,  
and in case of memory allocation failure, it returns a value indicating an error.
*/

int	ft_set_command(t_cmd **commands)
{
	t_cmd	*new;
	t_cmd	*tmp;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (ft_perror_message());
	new->next = NULL;
	new->append = false;
	new->infile = NULL;
	new->outfile = NULL;
	new->cmd = NULL;
	new->redir_error = false;
	new->here_doc = false;
	if (!(*commands))
		*commands = new;
	else
	{
		tmp = *commands;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (0);
}
/*
The `ft_redir` function handles redirections during token processing.  
It checks if the current token is of a type other than `T_WORD` or `T_PIPE`,  
indicating a potential redirection.  
If the next token exists and is a word (`T_WORD`),  
it calls the `ft_set_redir` function to set the appropriate redirection for the current command.  
In case of an error or lack of valid tokens, it frees the resources (tokens and commands)  
and returns an error code, preventing incorrect command configurations  
in the event of issues with redirections.
*/

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

/*
The `ft_pipe` function handles `T_PIPE` tokens by moving to the next token  
and updating the command when this token is encountered.  
If the token is of type `T_PIPE`, it calls the `ft_set_command` function  
to set the command, and if this function returns an error, it frees resources  
(tokens and commands) and returns `-1`. Otherwise, if everything is successful,  
it moves to the next token and command. If the initial token is `NULL`, it returns `0`.
*/

int	ft_pipe(t_token **current_tok, t_token *head_tok,
t_cmd **current_cmd, t_cmd *head_cmd)
{
	if (!*current_tok)
		return (0);
	if ((*current_tok)->type == T_PIPE)
	{
		if (ft_set_command(current_cmd) == -1)
		{
			ft_free_tokens(&head_tok);
			ft_free_commands(&head_cmd);
			return (-1);
		}
		(*current_tok) = (*current_tok)->next;
		(*current_cmd) = (*current_cmd)->next;
	}
	return (0);
}
