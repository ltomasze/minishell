/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:08:10 by mbany             #+#    #+#             */
/*   Updated: 2025/01/23 19:46:22 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `free_resources` zwalnia zasoby przypisane do struktury `data`. 
Najpierw czyści historię poleceń za pomocą `rl_clear_history`. 
Następnie, jeśli istnieje `cmd`, wywołuje funkcję `ft_free_commands` 
do jej zwolnienia. Jeśli istnieje tablica `envp_arr`, 
jest ona zwalniana przez `free_ft_split`. 
Na końcu, funkcja zwalnia listę zmiennych środowiskowych 
`envp` za pomocą `free_envp`. Po każdej operacji ustawia 
odpowiednie wskaźniki na `NULL`, aby zapobiec 
niekontrolowanemu dostępowi do zwolnionej pamięci.
*/
void	free_resources(t_data *data)
{
	rl_clear_history();
	if (data->cmd != NULL)
	{
		ft_free_commands(&(data->cmd));
		data->cmd = NULL;
	}
	if (data->envp_arr)
	{
		free_ft_split(data->envp_arr);
		data->envp_arr = NULL;
	}
	free_envp(data->envp);
	data->envp = NULL;
}

int	read_line(t_data *data)
{
	data->line = readline("minishell> ");
	if (!data->line)
	{
		printf("exit\n");
		free_resources(data);
		exit(0);
	}
	return (0);
}

/*
!data->line gdy jest NULL czyli gdy użyto 
Ctrl+D(EOF) lub gdy był error na readline
Funkcja `init` inicjalizuje strukturę `t_data` 
i przygotowuje środowisko dla programu.
*/
void	init(t_data *data, int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc != 1)
	{
		perror(MANY_ARGS_ERR);
		exit(-1);
	}
	if (envp == NULL || *envp == NULL)
	{
		perror(NO_ENVP_ERR);
		exit(-1);
	}
	data->envp = fetch_envp(envp);
	increment_shlvl(data->envp);
	data->cmd = NULL;
	data->envp_arr = NULL;
	data->line = NULL;
	data->cmd_exit_status = 0;
}

/*
Funkcja `check_for_builtins` sprawdza, 
czy komenda w strukturze `data->cmd` to jedna z komend 
wbudowanych (jak `exit`, `export`, `unset`, czy `cd`). 
Jeśli tak, wywołuje odpowiednią funkcję wbudowaną, 
a w przypadku `exit` kończy program, 
w innych przypadkach ustawia status zakończenia komendy 
(`data->cmd_exit_status`). Jeśli nie ma komendy 
lub występuje komenda kolejna (`next`), funkcja nie wykonuje żadnych działań.
*/
void	check_for_builtins(t_data *data)
{
	if (data->cmd->next != NULL || !data->cmd->cmd)
		return ;
	if (data->cmd->cmd && ft_strncmp(data->cmd->cmd[0], "exit", 5) == 0)
		exit_bltin(data);
	else if
		(ft_strncmp(data->cmd->cmd[0], "export",
		ft_strlen(data->cmd->cmd[0])) == 0)
		data->cmd_exit_status = export_bltin(data->cmd->cmd, data);
	else if
		(ft_strncmp(data->cmd->cmd[0], "unset",
		ft_strlen(data->cmd->cmd[0])) == 0)
		data->cmd_exit_status = unset_bltin(data->cmd->cmd, data);
	else if
		(ft_strncmp(data->cmd->cmd[0], "cd", ft_strlen(data->cmd->cmd[0])) == 0)
		data->cmd_exit_status = cd_bltin(data->cmd->cmd, data);
}

/*envp == NULL we check if we have environment variables*/
int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init(&data, argc, argv, envp);
	while (1)
	{
		handle_signals();
		if (read_line(&data))
			continue ;
		if (data.line && *data.line)
			add_history(data.line);
		if (check_syntax(data.line))
		{
			free(data.line);
			continue ;
		}
		sanitize_line(&data);
		if (!data.line)
			continue ;
		if (ft_commands_creation(&data) == -1)
			continue ;
		check_for_builtins(&data);
		execute_cmds(&data);
		ft_free_commands(&(data.cmd));
	}
	free_resources(&data);
}
