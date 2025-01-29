/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute01.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:34:09 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:17:42 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	set_status_and_msg_err(char *err, int code, int *status);

static int	check_for_dot_builtin(char *cmd, int *status);
/*
Funkcja `find_correct_path` szuka pełnej ścieżki do 
wykonania podanej komendy `cmd` na podstawie zmiennej środowiskowej `PATH`. 
Rozdziela wartość zmiennej `PATH` na osobne ścieżki, 
łączy każdą z nich z nazwą komendy i sprawdza, 
czy wynikowa ścieżka wskazuje na plik wykonywalny. 
Jeśli taką znajdzie, zwraca jej pełną ścieżkę; 
w przeciwnym razie zwraca `NULL`. Funkcja pozwala lokalizować 
i uruchamiać polecenia znajdujące się w katalogach 
zdefiniowanych w zmiennej `PATH`.
*/

static char	*find_correct_path(t_envp *envp, char *cmd)
{
	char	*final_envp_path;
	char	**envp_paths;
	char	*envp_path_part;
	int		i;

	if (!envp || cmd[0] == '.')
		return (NULL);
	envp_path_part = ft_strjoin("/", cmd);
	envp_paths = ft_split(envp->value + 5, ':');
	i = 0;
	final_envp_path = NULL;
	while (envp_paths[i] != NULL)
	{
		final_envp_path = ft_strjoin(envp_paths[i], envp_path_part);
		if (access(final_envp_path, X_OK) == 0)
			break ;
		free(final_envp_path);
		final_envp_path = NULL;
		i++;
	}
	free_ft_split(envp_paths);
	free(envp_path_part);
	return (final_envp_path);
}

/*
Funkcja `find_cmd_path` znajduje pełną ścieżkę do polecenia, 
sprawdzając najpierw, czy jest to wbudowane polecenie (sprawdzenie doty), 
a następnie czy polecenie jest plikiem wykonywalnym. 
Jeśli nie ma odpowiednich uprawnień, ustawia odpowiedni kod błędu. 
Następnie przeszukuje zmienne środowiskowe (szukając zmiennej `PATH`), 
aby znaleźć ścieżkę, w której polecenie może być wykonywane. 
Jeśli nie uda się znaleźć ścieżki lub polecenie nie istnieje, 
ustawia błąd i zwraca `NULL`. Funkcja ta jest częścią projektu, 
który odwzorowuje zachowanie powłoki, 
w tym zarządzanie błędami przy wykonywaniu poleceń.
*/
char	*find_cmd_path(t_envp *envp, char *cmd, int *status)
{
	char	*final_envp_path;

	if (check_for_dot_builtin(cmd, status))
		return (NULL);
	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
		return (cmd);
	else if (access(cmd, F_OK) == 0 && access(cmd, X_OK) != 0)
	{
		set_status_and_msg_err(NO_PERM_ERR, 126, status);
		free(cmd);
		return (NULL);
	}
	while (envp && ft_strncmp(envp->value, "PATH", 4) != 0)
		envp = envp->next;
	final_envp_path = find_correct_path(envp, cmd);
	if (final_envp_path == NULL)
	{
		set_status_and_msg_err(NO_CMD_ERR, 127, status);
		free(cmd);
		return (NULL);
	}
	free(cmd);
	return (final_envp_path);
}

/*
Funkcja `set_exit_status` analizuje kod zakończenia procesu 
zapisany w zmiennej `status` i ustawia odpowiedni kod zakończenia 
w `cmd_exit_status`. Jeśli proces zakończył się normalnie, 
zapisuje jego kod wyjścia. 
W przypadku zakończenia sygnałem dodaje wartość 128 do numeru sygnału. 
Gdy proces został zatrzymany lub wznowiony, również ustawia odpowiedni kod. 
Funkcja ta pozwala na poprawne odwzorowanie 
i przekazanie kodów zakończenia procesów w powłoce, 
co jest kluczowe do obsługi błędów i sygnałów w projekcie.
*/
void	set_exit_status(int *cmd_exit_status, int status)
{
	if (WIFEXITED(status))
		*cmd_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*cmd_exit_status = 128 + WTERMSIG(status);
	else if (WIFSTOPPED(status))
		*cmd_exit_status = 128 + WSTOPSIG(status);
	else if (WIFCONTINUED(status))
		*cmd_exit_status = 128;
	else
		*cmd_exit_status = -1;
}

/*
Funkcja `check_for_dot_builtin` sprawdza, 
czy podana komenda to specjalny przypadek `"."` 
lub `".."`. Jeśli tak, ustawia odpowiedni kod błędu i komunikat, 
zwalnia pamięć przydzieloną dla komendy i zwraca wartość `1`, 
co oznacza, że komenda nie powinna być dalej wykonywana. 
W przeciwnym razie zwraca `0`. Funkcja zapobiega błędom, 
gdy użytkownik próbuje użyć nieprawidłowych poleceń systemowych `"."` 
lub `".."`, co zwiększa zgodność z zachowaniem powłoki.
*/
static int	check_for_dot_builtin(char *cmd, int *status)
{
	if (ft_strncmp(cmd, ".", 2) == 0)
	{
		set_status_and_msg_err(NO_FNAME_ARG_ERR, 2, status);
		free(cmd);
		return (1);
	}
	else if (ft_strncmp(cmd, "..", 3) == 0)
	{
		set_status_and_msg_err(NO_CMD_ERR, 127, status);
		free(cmd);
		return (1);
	}
	return (0);
}

static void	set_status_and_msg_err(char *err, int code, int *status)
{
	*status = code;
	msg_error(err);
}
