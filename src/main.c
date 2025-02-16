/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:47:23 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:49:36 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The free_resources function frees resources assigned to the data structure.
It first clears the command history using rl_clear_history. Then, if cmd exists,
it calls the ft_free_commands function to free it. If envp_arr exists,
it is freed using free_ft_split. Finally, it frees the list of environment variables
envp using free_envp. After each operation, it sets the corresponding pointers to NULL
to prevent uncontrolled access to freed memory.
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
!data->line occurs when it's NULL, which happens when Ctrl+D (EOF) is used
or when there's an error in readline.
The init function initializes the t_data structure and prepares the environment
for the program.
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
The check_for_builtins function checks if the command in the data->cmd structure
is one of the built-in commands (like exit, export, unset, or cd).
If so, it calls the corresponding built-in function.
For the exit command, it terminates the program,
while for other cases, it sets the command's exit status (data->cmd_exit_status).
If there's no command or a next command (next), the function performs no actions.
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
