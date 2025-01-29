/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:27:24 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 17:31:59 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_here_doc_cmd(t_cmd *current_cmd, char *str);
static void	ft_append_cmd(t_cmd *current_cmd, char *str);
static void	ft_redir_out_cmd(t_cmd *current_cmd, char *str);
static void	ft_redir_in_cmd(t_cmd *current_cmd, char *str);

/*
Funkcja `ft_set_redir` ustawia odpowiednią redirekcję 
w bieżącym poleceniu na podstawie typu tokenu. 
Jeśli w poleceniu wystąpił wcześniej błąd redirekcji, nic nie robi. 
Tworzy kopię tekstu z następnego tokenu 
(ścieżka do pliku lub znak redirekcji) 
i w zależności od typu bieżącego tokenu 
(`T_IN_REDIR`, `T_OUT_REDIR`, `T_APPEND`, `T_HEREDOC`), 
wywołuje odpowiednią funkcję (`ft_redir_in_cmd`, `ft_redir_out_cmd`, 
`ft_append_cmd`, `ft_here_doc_cmd`) 
w celu ustawienia redirekcji w strukturze `t_cmd`. 
W razie problemu z alokacją pamięci zwraca błąd.
*/
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
Funkcja `ft_redir_in_cmd` obsługuje ustawianie pliku wejściowego
 dla polecenia w strukturze `t_cmd`. 
Sprawdza, czy plik `str` jest dostępny do odczytu; jeśli nie, 
ustawia błąd redirekcji w polu `redir_error`, 
zwalnia pamięć dla `str` i kończy działanie. 
Jeśli dostęp jest poprawny, zwalnia poprzedni plik wejściowy (jeśli istnieje), 
przypisuje nowy plik do `infile` i ustawia `here_doc` na `false`, 
aby oznaczyć brak użycia heredoc.
*/
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

/*
Funkcja `ft_redir_out_cmd` ustawia plik wyjściowy 
dla polecenia w strukturze `t_cmd`. 
Próbuje otworzyć plik `str` w trybie zapisu, tworząc go, 
jeśli nie istnieje, i usuwając jego zawartość, jeśli istnieje. 
Jeśli operacja otwarcia zakończy się niepowodzeniem, 
wypisuje komunikat błędu, ustawia flagę błędu redirekcji 
`redir_error` i zwalnia pamięć dla `str`. 
Jeśli otwarcie się powiedzie, zamyka deskryptor pliku, 
zwalnia ewentualny poprzedni plik wyjściowy, 
przypisuje nowy do `outfile` i ustawia `append` na `false`, 
aby wskazać, że dane mają być nadpisywane.
*/
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

/*
Funkcja `ft_append_cmd` otwiera plik `str` 
w trybie dopisywania (O_APPEND) lub tworzy go, 
jeśli nie istnieje (O_CREAT). 
Jeśli otwarcie się nie powiedzie, 
ustawia flagę błędu przekierowania w strukturze `current_cmd`, 
wyświetla komunikat błędu i zwalnia pamięć dla `str`. 
W przeciwnym razie zamyka plik, 
aktualizuje wskaźnik `outfile` w `current_cmd` do nowego pliku `str` 
i ustawia tryb dopisywania (`append`) na `true`.
*/
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

/*
Funkcja `ft_here_doc_cmd` ustawia w strukturze 
`current_cmd` plik wejściowy na `str`, 
zwalniając wcześniej zajmowaną pamięć przez 
poprzedni plik wejściowy, jeśli istniał, oraz oznacza, 
że używane jest polecenie typu here document, 
ustawiając flagę `here_doc` na `true`.
*/
static void	ft_here_doc_cmd(t_cmd *current_cmd,
char *str)
{
	if (current_cmd->infile)
		free(current_cmd->infile);
	current_cmd->infile = str;
	current_cmd->here_doc = true;
}
