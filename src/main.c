/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:08:10 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 18:47:12 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	check_for_builtins(t_data *data)
{
	if (data->cmd->next != NULL || !data->cmd->cmd)
		return ;
	if (data->cmd->cmd && ft_strncmp(data->cmd->cmd[0],
			"exit", 5) == 0)
		exit_bltin(data);
	else if (ft_strncmp(data->cmd->cmd[0],
			"export", ft_strlen(data->cmd->cmd[0])) == 0)
		data->cmd_exit_status = export_bltin(data->cmd->cmd, data);
	else if (ft_strncmp(data->cmd->cmd[0],
			"unset", ft_strlen(data->cmd->cmd[0])) == 0)
		data->cmd_exit_status = unset_bltin(data->cmd->cmd, data);
	else if (ft_strncmp(data->cmd->cmd[0],
			"cd", ft_strlen(data->cmd->cmd[0])) == 0)
		data->cmd_exit_status = cd_bltin(data->cmd->cmd, data);
}
/*if (data->cmd->cmd... czy komenda wogóle istnieje*/

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

/* !data->line gdy jest NULL czyli gdy użyto Ctrl+D(EOF) lub gdy był error na readline*/
// Funkcja `init` inicjalizuje strukturę `t_data` i przygotowuje środowisko dla programu.

void	init(t_data *data,int argc,char **argv,char **envp)
{
	(void)argv;
	if(argc != 1)
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
		sanitaze_line(&data);
		if (!data.line)
			continue ;
		if (ft_cmds_creation(&data) == -1)
			continue ;
		check_for_builtins(&data);
		execute_cmds(&data);
		ft_free_commands(&(data.cmd));
	}
	free_resources(&data);
}

/* if (read_line(&data))      // Wczytuje linię
        continue;              // Jeśli wczytano linię, kontynuuj

data.line to zmienna dynamiczna używana do przechowywania
aktualnej linii wejściowej wprowadzonej przez użytkownika np. ls -la

Kiedy używamy data.line vs. data->line?
data.line: Używamy, gdy mamy bezpośredni obiekt struktury, np.:
t_data data; // Zmienna na stosie
data.line = readline("minishell> ");
data->line: Używamy, gdy mamy wskaźnik do struktury, np.:
t_data *data = malloc(sizeof(t_data)); // Zmienna w dynamicznej pamięci
data->line = readline("minishell> ");
Dlaczego to ważne?
Operator . działa na samą strukturę.
Operator -> działa na wskaźnik do struktury.
*/