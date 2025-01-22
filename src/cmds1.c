/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:27:30 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/22 18:13:44 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_free_args(int i, char **cmds)
{
	int	x;

	ft_perror_message();
	x = 0;
	while (x < i)
		free(cmds[x++]);
	free(cmds);
	return (-1);
}

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
tokens: wskaźnik na listę tokenów reprezentujących aktualną komendę.
commands: wskaźnik na strukturę t_cmd, w której zapiszemy tablicę argumentów (cmd).
i: liczba tokenów (argumentów) w komendzie, przekazana np. z funkcji ft_count_tok.
cmds: Wskaźnik na tablicę argumentów (komenda + jej parametry).
arg: Tymczasowy wskaźnik przechowujący kopię tekstu z tokenu.
n: Licznik zapełnionych miejsc w tablicy cmds

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
/*najpierw sprawdzamy czy token nie jest | bo | kończy komende
później sprawdzamy czy token nie jest słowem, bo jeżeli nie jest
to uwążamy jest to redir wiec trzeba zrobić +2 aby przejść redir
i nazwe pliku
token który jest T_WORD następnie zmieniamy T_ARG ponieważ
nie jest to już komenda tylko argument*/

int	ft_command(t_token **cur_token, t_token *tokens,
	t_cmd **cur_command, t_cmd *cmds)
{
	int	i;

	if (!(*cur_token))
		return (0);
	if ((*cur_token)->type == T_WORD)
	{
		i = ft_count_tok(*cur_token);
		if (ft_create_cmds(*cur_token, *cur_command, i) == -1)
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
if ((*cur_token)->type == T_WORD) czyli czy token to T_WORD
jak ls lub echo, jeśli tak to 
i = ft_count_tok(*cur_token); np jeśli pierwszy token do ls to sprawdzamy
ile jeszcze jest tokenów poza ls typu T_ARG, czyli -l(T_ARG) -a(T_ARG) razem
ft_count_tok zwróci 3(komenda + 2 argumenty), następnie uruchamia komendę
ft_create_cmds która tworzy tablicę argumentów na podstawie tokenów i 
przypisuje je do struktury t_cmd. np. cmd: "ls args["ls", "-l", "-a"]
if ((*cur_token)->type == T_ARG) do przechodzenia przez wszystkie argumenty do
danej komendy aż trafimy na inny typ jak np. pip czyli | to wtedy przechodzimy
do innej funkcji która ogrania taki pipe
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