/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:07:34 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/25 19:25:23 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pwd_bltin(void)
{
	char	path[4096];

	if (!(getcwd(path, 4096)))
	{
		ft_perror_message();
		exit(1);
	}
	else
		printf("%s\n", path);
	exit(0);
}

static void	ft_echo_args(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		i++;
		if (cmd[i])
			printf(" ");
	}
}

void	echo_bltin(char **cmd)
{
	int		i;
	int		j;
	bool	flag;

	i = 1;
	j = 0;
	flag = false;
	while (cmd[i])
	{
		if (!cmd[i][j] || cmd[i][j] != '-')
			break ;
		j++;
		while (cmd[i][j] && cmd[i][j] == 'n')
			j++;
		if (cmd[i][j] || j == 1)
			break ;
		j = 0;
		flag = true;
		i++;
	}
	ft_echo_args(&cmd[i]);
	if (flag == false)
		printf("\n");
	exit(0);
}

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
