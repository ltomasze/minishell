/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:40:51 by mbany             #+#    #+#             */
/*   Updated: 2025/01/23 19:52:40 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `exit_bltin` obsługuje polecenie `exit` wbudowane w shellu. 
Sprawdza, czy podano argument (kod wyjścia). 
Jeśli tak, weryfikuje, czy jest to liczba 
(z opcjonalnym znakiem minus na początku), 
i jeśli nie, wyświetla komunikat o błędzie, 
zwalnia zasoby i kończy program z kodem wyjścia `2`. 
Jeśli argument jest poprawny, zamienia go na liczbę za pomocą `ft_atoi`. 
Gdy brak argumentu, używa domyślnego kodu wyjścia. 
Następnie zwalnia zasoby i zamyka program z odpowiednim kodem wyjścia.
*/
void	exit_bltin(t_data *data)
{
	int	exit_status;
	int	i;

	if (data->cmd->cmd[1] != NULL)
	{
		i = 0;
		while (data->cmd->cmd[1][i] != '\0')
		{
			if (!ft_isdigit(data->cmd->cmd[1][i])
				&& !(i == 0 && data->cmd->cmd[1][i] == '-'))
			{
				msg_error(NUM_REQ_ERR);
				free_resources(data);
				exit(2);
			}
			i++;
		}
		exit_status = ft_atoi(data->cmd->cmd[1]);
	}
	else
		exit_status = data->cmd_exit_status;
	free_resources(data);
	exit(exit_status);
}

/*
Funkcja `check_for_builtin_and_execute` sprawdza, 
czy polecenie przekazane jako argument `cmd` jest 
jednym z wbudowanych poleceń powłoki, takich jak `exit`, 
`env`, `echo`, `pwd`, `cd`, `unset`, czy `export`, 
i wykonuje odpowiednią funkcję implementującą dane polecenie. 
Jeśli polecenie jest wbudowane, zostaje ono obsłużone natychmiast 
w procesie rodzica (bez tworzenia procesu podrzędnego). 
Funkcja zwraca `-1`, jeśli brak polecenia, a `0`, 
jeśli wykonano wbudowane polecenie. Jest to kluczowe 
w projekcie *minishell*, aby obsłużyć wbudowane polecenia 
zgodnie z wymaganiami i bez uruchamiania ich jako oddzielnych procesów, 
co różni je od poleceń zewnętrznych. 
*/
int	check_for_builtin_and_execute(char **cmd, t_data *data)
{
	if (!cmd || !cmd[0])
		return (-1);
	if (ft_strncmp(cmd[0], "exit", ft_strlen(cmd[0])) == 0)
		exit_bltin(data);
	else if (ft_strncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0)
		env_bltin(data);
	else if (ft_strncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
		echo_bltin(cmd);
	else if (ft_strncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0)
		pwd_bltin();
	else if (ft_strncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0)
		exit (data->cmd_exit_status);
	else if (ft_strncmp(cmd[0], "unset", ft_strlen(cmd[0])) == 0)
		exit(data->cmd_exit_status);
	else if (ft_strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0)
		exit(ft_print_env_var(data));
	return (0);
}
