/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:16:10 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:16:13 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The function exit_bltin handles the exit built-in command in the shell.
It checks if an argument (exit code) is provided.
If so, it verifies whether the argument is a number (with an optional minus sign at the beginning), and if not, it displays an error message, frees resources, and exits the program with exit code 2.
If the argument is valid, it converts it to a number using ft_atoi.
If no argument is provided, the default exit code is used.
Then, it frees resources and closes the program with the appropriate exit code.
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
The function check_for_builtin_and_execute checks if the command passed as the argument cmd is one of the built-in shell commands, such as exit, env, echo, pwd, cd, unset, or export, and executes the corresponding function implementing the command.
If the command is built-in, it is handled immediately in the parent process (without creating a child process).
The function returns -1 if the command is not found and 0 if a built-in command is executed.
This is crucial in the minishell project to handle built-in commands according to the requirements, without running them as separate processes, which distinguishes them from external commands.
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
