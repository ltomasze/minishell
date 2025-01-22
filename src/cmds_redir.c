/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:27:53 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/22 16:45:45 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_here_doc_cmd(t_cmd *current_cmd, char *str)
{
	if (current_cmd->infile)
		free(current_cmd->infile);
	current_cmd->infile = str;
	current_cmd->here_doc = true;
}

static void	ft_append_cmd(t_cmd *current_cmd, char *str)
{
	int	fd;

	fd = open(str, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_perror_message();
		current_cmd->redir_error = true;
		free(str);
		return ;
	}
	close(fd);
	if (current_cmd->outfile)
		free(current_cmd->outfile);
	current_cmd->outfile = str;
	current_cmd->append = true;
}

static void	ft_redir_out_cmd(t_cmd *current_cmd, char *str)
{
	int	fd;

	fd = open(str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_perror_message();
		current_cmd->redir_error = true;
		free(str);
		return ;
	}
	close(fd);
	if (current_cmd->outfile)
		free(current_cmd->outfile);
	current_cmd->outfile = str;
	current_cmd->append = false;
}
/*O_WRONLY plik tylko do Write-Only
O_TRUNC jeśli istnieje plik to zostanie przycięty do zera tj. dane zostaną
usunięte i możliwe będzie ich nadpisanie, używamy do nadpisywania*/
/*O_CREAT jeśli plik nie istnieje zostanie utworzony*/
/* 0644 wartość określając uprawnienia pliku
0 oznacza zapis w systemie ósemkowym(bez zera syst. dziesiątkowy),
 6 właściciel, 4 grupa, 4 pozostali użytkownicy
zapis ósemkowy to trzy bity czyli 2do3=8
6 binarnie to 110 czyli 1=r(odczyt), 1=w(pisanie), 0=- brak wykonania
110(1 bo w 6 mieści się jedno 4, 1 bo mieścu się jedno 2,
0 bo nie mieści się już 1, gdyby było 7 to liczba byłaby 111
trzecie 1 daje uprawnienia execute, całość to rwx)
4 to 100, czyli 1=r, 0=-, 0=-*/
/*fd = open(str, O_WRONLY | O_APPEND | O_CREAT, 0644); | służy do łączenia
flag*/
/* close(fd) bo po udanym otwarciu można plik zamknąć gdy sprawdziliśmy dostęp
do niego*/
/*current_cmd->outfile = str; ustawiam nazwę nowego pliku
> nadpisanie pliku, blokujeme >> dopisanie pliku*/

static void	ft_redir_in_cmd(t_cmd *current_cmd, char *str)
{
	if (ft_check_access(str, READ) < 0)
	{
		current_cmd->redir_error = true;
		free(str);
		return ;
	}
	if (current_cmd->infile)
		free(current_cmd->infile);
	current_cmd->infile = str;
	current_cmd->here_doc = false;
}
/*if (current_cmd->infile)
		free(current_cmd->infile);
	jeśli nasz plik wejściowy tj. obecny, był wcześniej wyjściowym, to
	czyścimy go;
	current_cmd->infile = str;
	current_cmd->here_doc = false;
	bo używając < blokujemy od razu << 
	cat < input.txt polecenie cat czyta z pliku input.txt
	cat << input.txt
	Hello
	input.txt to wynik Hello czyli czyta od heredoc do ustawionego
	miejsca jak wyżej lub np. EOF*/

int	ft_set_redir(t_token **current_tok, t_cmd *current_cmd)
{
	char	*str;

	if (current_cmd->redir_error == true)
		return (0);
	str = ft_strdup((*current_tok)->next->text);
	if (!str)
		return (ft_perror_message());
	if ((*current_tok)->type == T_IN_REDIR)
		ft_redir_in_cmd(current_cmd, str);
	if ((*current_tok)->type == T_OUT_REDIR)
		ft_redir_out_cmd(current_cmd, str);
	if ((*current_tok)->type == T_APPEND)
		ft_append_cmd(current_cmd, str);
	if ((*current_tok)->type == T_HEREDOC)
		ft_here_doc_cmd(current_cmd, str);
	return (0);
}
/*
T_IN_REDIR <
T_OUT_REDIR >
T_APPEND – Redirekcja wyjścia z dopisaniem >>
T_HEREDOC – <<
*/