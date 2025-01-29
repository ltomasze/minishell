/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_descriptor_handlers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 11:51:13 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 17:02:08 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `update_input_fd` obsługuje 
i aktualizuje deskryptor pliku wejściowego dla komendy, 
sprawdzając różne przypadki: jeśli `here_doc` jest ustawiony, 
wywołuje `get_heredoc`, 
aby utworzyć potok dla wprowadzenia; jeśli wystąpił błąd przekierowania, 
zwraca -1 jako sygnał błędu; jeśli podano plik wejściowy 
i nie ma błędu przekierowania, otwiera plik w trybie odczytu, 
aktualizując deskryptor. 
Funkcja zapewnia poprawne źródło danych wejściowych 
dla komend w potoku, obsługując pliki i here-dok.
*/
int	update_input_fd(t_cmd *cmd, int input_fd)
{
	if (cmd->here_doc)
	{
		input_fd = get_heredoc(cmd);
		if (input_fd < 0)
			ft_error_message(HEREDOC_ERR, -1);
		return (input_fd);
	}
	else if (cmd->redir_error == true)
		return (-1);
	else if (cmd->infile && cmd->redir_error == false)
	{
		input_fd = open(cmd->infile, O_RDONLY);
		if (input_fd < 0)
			perror(cmd->infile);
		return (input_fd);
	}
	return (input_fd);
}

/*
Funkcja `get_output_fd` ustala deskryptor pliku wyjściowego dla danej komendy. 
Jeśli komenda ma przypisany plik wyjściowy, otwiera go w trybie nadpisywania 
lub dołączania w zależności od flagi `append`. 
Jeśli nie ma pliku wyjściowego i komenda jest ostatnia, 
używa standardowego wyjścia. W przeciwnym razie używa końca zapisu w potoku. 
Zapewnia, że każda komenda w potoku ma odpowiednio zdefiniowany cel wyjściowy, 

a w przypadku błędu otwarcia pliku generuje komunikat diagnostyczny.
*/
int	get_output_fd(t_cmd *cmd, int *fd_pipe)
{
	int	output_fd;

	if (cmd->outfile && cmd->append == false)
		output_fd = open(cmd->outfile,
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else if (cmd->outfile && cmd->append == true)
		output_fd = open(cmd->outfile,
				O_WRONLY | O_CREAT | O_APPEND, 0664);
	else if (cmd->next == NULL)
		output_fd = STDOUT_FILENO;
	else
		output_fd = fd_pipe[1];
	if (output_fd < 0 && cmd->outfile)
		perror(cmd->outfile);
	else if (output_fd < 0 && !cmd->outfile)
		perror("output_fd");
	return (output_fd);
}

/*
Funkcja `env_bltin` obsługuje wbudowaną komendę `env`, 
która wypisuje wszystkie zmienne środowiskowe zapisane w liście `data->envp`. 
Jeśli do komendy `env` podano dodatkowe argumenty, 
funkcja wyświetla błąd "env: Too many arguments" 
i kończy działanie z kodem błędu `1`. 
W przeciwnym razie iteruje przez listę zmiennych środowiskowych 
i wypisuje każdą z nich. 
Funkcja kończy proces, zwracając kod zakończenia `0`. 
Jest to wymagane w *minishell*, by zapewnić poprawne działanie 
i wyświetlanie zmiennych środowiskowych zgodnie ze specyfikacją.
*/
void	env_bltin(t_data *data)
{
	t_envp	*envp;

	if (data->cmd->cmd[1])
	{
		ft_error_message("env: Too many arguments", 2);
		exit(1);
	}
	envp = data->envp;
	while (envp)
	{
		printf("%s\n", envp->value);
		envp = envp->next;
	}
	exit(0);
}

/*
Funkcja `get_heredoc` odczytuje dane w trybie "here-document" 
z wejścia standardowego, zapisując je do pipe, 
aż napotka specjalny delimiter (wskazany przez `eof`). 
Zwraca deskryptor pliku rury, 
który będzie użyty jako wejście dla kolejnej komendy. 
Używana jest w przypadku, gdy użytkownik chce przekazać dane do komendy, 
zakończone specjalnym wskaźnikiem (np. `EOF`). 
Celem jest obsługa dynamicznych danych wejściowych w potokach.
*/
int	get_heredoc(t_cmd *cmd)
{
	int		fd_pipe[2];
	char	*input;
	char	*eof;

	eof = cmd->infile;
	pipe(fd_pipe);
	while (1)
	{
		write(1, "> ", 2);
		input = get_next_line(STDIN_FILENO);
		if (!input)
		{
			printf("\nWarning: here_doc delimited by EOF signal\n");
			break ;
		}
		if (ft_strncmp(input, eof, ft_strlen(eof)) == 0
			&& input[ft_strlen(eof)] == '\n')
			break ;
		write(fd_pipe[1], input, ft_strlen(input));
		free(input);
	}
	free(input);
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}
