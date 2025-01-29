/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute00.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:34:09 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:14:36 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	process_last_cmd_child(t_data *data, t_cmd *cmd_node, int input_fd);
static void	process_last_cmd(t_data *data, t_cmd *cmd_node, int input_fd);
static void	process_cmd(t_data *data, t_cmd *cmd_node,
				int input_fd, int *fd_pipe);

/*
Funkcja `execute_cmds` odświeża tablicę zmiennych środowiskowych 
na podstawie aktualnej listy połączonej, 
a następnie wywołuje funkcję `recursive_pipeline`, 
aby wykonać polecenia w potoku. Jest używana w projekcie *Minishell*, 
aby zapewnić synchronizację stanu zmiennych środowiskowych 
i obsłużyć złożone struktury potoków w trakcie wykonywania poleceń.
*/
void	execute_cmds(t_data *data)
{
	if (data->envp_arr)
	{
		free_ft_split(data->envp_arr);
		data->envp_arr = NULL;
	}
	data->envp_arr = convert_envp_list_to_array(data->envp);
	recursive_pipeline(0, data, data->cmd);
}

/*
Funkcja `recursive_pipeline` implementuje przetwarzanie potoków 
(*pipelines*) w poleceniach w *Minishell*. Działa rekurencyjnie, 
tworząc kolejne potoki za pomocą funkcji `pipe` 
i procesów potomnych za pomocą `fork`. Jeśli `cmd_node` jest ostatnim w potoku, 
wywołuje funkcję `process_last_cmd`. 
W przeciwnym razie przetwarza aktualne polecenie 
w procesie potomnym za pomocą `process_cmd`, 
przekazując dane przez potok do następnego procesu. 
Proces nadrzędny zarządza zamykaniem plików 
i czeka na zakończenie procesów potomnych. 
Funkcja pozwala na wykonanie ciągu powiązanych poleceń, 
gdzie wyjście jednego jest wejściem dla kolejnego, 
co jest kluczową funkcjonalnością w powłoce zgodnej z UNIX-em.
*/
void	recursive_pipeline(int input_fd, t_data *data, t_cmd *cmd_node)
{
	int		fd_pipe[2];
	int		status;
	pid_t	pid;

	if (cmd_node->next == NULL)
		process_last_cmd(data, cmd_node, input_fd);
	else
	{
		pipe(fd_pipe);
		pid = fork();
		if (pid < 0)
			perror("fork failed");
		else if (pid == 0)
			process_cmd(data, cmd_node, input_fd, fd_pipe);
		else
		{
			signal(SIGINT, SIG_IGN);
			close(fd_pipe[1]);
			if (input_fd > 0)
				close(input_fd);
			recursive_pipeline(fd_pipe[0], data, cmd_node->next);
			waitpid(pid, &status, 0);
		}
	}
}

/*
Funkcja `process_last_cmd_child` obsługuje wykonanie 
ostatniego polecenia w potoku w procesie potomnym w *Minishell*. 
Ustawia domyślne sygnały, aktualizuje deskryptory wejścia i wyjścia, 
wykonuje zduplikowanie deskryptorów dla wejścia 
i wyjścia procesu oraz zamyka niepotrzebne deskryptory. 
Jeśli polecenie jest funkcją wbudowaną, wywołuje ją, 
a jeśli nie, szuka ścieżki do wykonywalnego pliku 
w zmiennych środowiskowych i wywołuje `execve`, 
aby uruchomić program z podanym środowiskiem. 
W przypadku błędu w wykonaniu wyświetla komunikat diagnostyczny 
i kończy proces z odpowiednim statusem. 
Funkcja jest kluczowa dla poprawnego obsłużenia ostatniego polecenia 
w potoku z właściwym zarządzaniem wejściem/wyjściem oraz obsługą błędów.
*/
static void	process_last_cmd_child(t_data *data, t_cmd *cmd_node, int input_fd)
{
	int	output_fd;
	int	status;

	set_signals_to_default();
	input_fd = update_input_fd(cmd_node, input_fd);
	output_fd = get_output_fd(cmd_node, NULL);
	duplicate_fds(input_fd, output_fd);
	if (input_fd > 0)
		close(input_fd);
	if (output_fd > 2)
		close(output_fd);
	if (!cmd_node->cmd)
		exit(0);
	check_for_builtin_and_execute(cmd_node->cmd, data);
	cmd_node->cmd[0] = find_cmd_path(data->envp, cmd_node->cmd[0], &status);
	if (cmd_node->cmd[0] && output_fd > 0 && cmd_node->redir_error == false)
		status = execve(cmd_node->cmd[0], cmd_node->cmd, data->envp_arr);
	if (status < 0)
		perror("execve failed");
	exit(status);
}

/*
Funkcja `process_last_cmd` w *Minishell* uruchamia ostatnie polecenie w potoku, 
tworząc nowy proces przy użyciu `fork`. 
W procesie potomnym wywołuje funkcję `process_last_cmd_child`, 
która realizuje wykonanie polecenia, zarządza wejściem/wyjściem 
i obsługuje błędy. Proces nadrzędny ignoruje sygnały przerwania, 
zamyka niepotrzebne deskryptory wejścia, 
czeka na zakończenie procesu potomnego i zapisuje status wyjścia polecenia. 
Funkcja jest niezbędna do obsługi ostatniego polecenia w potoku 
i zapewnia poprawną synchronizację procesów 
oraz ustawienie końcowego statusu wyjścia.
*/
static void	process_last_cmd(t_data *data, t_cmd *cmd_node, int input_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		perror("fork failed");
	else if (pid == 0)
		process_last_cmd_child(data, cmd_node, input_fd);
	else
	{
		signal(SIGINT, SIG_IGN);
		if (input_fd > 0)
			close(input_fd);
		waitpid(pid, &status, 0);
		set_exit_status(&(data->cmd_exit_status), status);
	}
}

/*
Funkcja `process_cmd` w *Minishell* obsługuje wykonanie 
pojedynczego polecenia w ramach potoku. Ustawia sygnały na domyślne, 
zarządza wejściem/wyjściem przez przekierowania i deskryptory plików, 
oraz zamyka niepotrzebne deskryptory w potoku. 
Jeśli polecenie jest wbudowane, wykonuje je od razu. 
W przeciwnym razie wyszukuje pełną ścieżkę do programu 
i uruchamia go za pomocą `execve`. Funkcja kończy działanie, 
gdy wystąpi błąd lub zakończy wykonanie polecenia. 
Jest kluczowa dla obsługi każdego kroku w potoku 
i zapewnia prawidłowe przetwarzanie danych oraz synchronizację procesów.
*/
static void	process_cmd(t_data *data, t_cmd *cmd_node,
	int input_fd, int *fd_pipe)
{
	int	output_fd;
	int	status;

	set_signals_to_default();
	input_fd = update_input_fd(cmd_node, input_fd);
	if (input_fd < 0)
	{
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		exit(1);
	}
	output_fd = get_output_fd(cmd_node, fd_pipe);
	duplicate_fds(input_fd, output_fd);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	if (input_fd > 0)
		close(input_fd);
	if (!cmd_node->cmd)
		exit(0);
	check_for_builtin_and_execute(cmd_node->cmd, data);
	cmd_node->cmd[0] = find_cmd_path(data->envp, cmd_node->cmd[0], &status);
	if (cmd_node->cmd[0] && input_fd >= 0 && cmd_node->redir_error == false)
		status = execve(cmd_node->cmd[0], cmd_node->cmd, data->envp_arr);
	exit(status);
}
