/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:01:14 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:06:09 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_cd_home(t_data *data, t_envp *home);
static int	cd_handler(char *str, t_data *data);
static int	ft_change_value(char *var, char *res, t_data *data);
static int	ft_cd_env_change(t_data *data, char *var, char *res);

/*
The function cd_bltin handles the cd command in the shell, changing the current working directory.
If more than one argument is provided, it returns an error "Too many arguments".
If no arguments are given, it attempts to change to the HOME directory by looking it up in the environment variables.
If one argument is provided, it uses it to change the directory with the cd_handler function.
In case of an error, it returns code 1, and on success, it returns 0.
*/
int	cd_bltin(char **cmd, t_data *data)
{
	t_envp	*node;

	if (cmd[1] && cmd[2])
		return (ft_error_message("Too many arguments", 1));
	else if (!cmd[1])
	{
		node = fetch_envp_node(data->envp, "HOME");
		return (ft_cd_home(data, node));
	}
	else if (cd_handler(cmd[1], data) == -1)
		return (1);
	return (0);
}

/*
The function ft_cd_home changes the current working directory to the user's home directory 
stored in the HOME environment variable.
If HOME is not set, the function returns an error.
After changing the directory, it updates the environment variables OLDPWD and PWD, 
which track the previous and current working directories.
It is crucial in the Minishell project because it implements the handling of the cd command with no arguments, 
allowing the user to quickly return to the home directory, in line with the behavior of standard system shells.
*/
static int	ft_cd_home(t_data *data, t_envp *home)
{
	t_envp	*node_pwd;
	char	curr[4096];

	if (!home)
		return (ft_error_message("HOME not set", 1));
	if (chdir(&home->value[5]))
	{
		ft_perror_message();
		return (1);
	}
	getcwd(curr, 4096);
	node_pwd = fetch_envp_node(data->envp, "PWD");
	if (!home)
	{
		if (ft_change_value("OLDPWD", NULL, data) == -1)
			return (1);
	}
	else
	{
		if (ft_change_value("OLDPWD", &node_pwd->value[4], data) == -1)
			return (1);
	}
	if (ft_change_value("PWD", curr, data) == -1)
		return (1);
	return (0);
}

/*
The function cd_handler changes the working directory to the one specified by the argument str.
If the directory change fails, an error message is displayed, and the function returns -1.
In case of success, the environment variables OLDPWD (set to the previous location)
and PWD (set to the new location) are updated.
This function is necessary in the Minishell project to handle the cd command 
with a given argument and ensure the proper updating of environment variables, 
in line with the behavior of standard system shells.
*/
static int	cd_handler(char *str, t_data *data)
{
	char	cur[4096];
	t_envp	*node;

	if (chdir(str) == -1)
	{
		ft_perror_message();
		return (-1);
	}
	node = fetch_envp_node(data->envp, "PWD");
	if (node)
	{
		if (ft_change_value("OLDPWD", &node->value[4], data) == -1)
			return (-1);
	}
	getcwd(cur, 4096);
	if (ft_change_value("PWD", cur, data) == -1)
		return (-1);
	return (0);
}

/*
The function ft_change_value changes the value of the environment variable named var to the new value res.
If the variable already exists, its value is updated; otherwise, a new node is created in the environment variable list.
The function uses dynamically allocated memory to create a new string that combines the variable name with its new value.
This is crucial in the Minishell project for proper management of environment variables, including, for example, 
when handling the change of the working directory (cd) and updating variables such as PWD or OLDPWD.
*/
static int	ft_change_value(char *var, char *res, t_data *data)
{
	t_envp	*node;
	char	*str;
	int		var_len;

	ft_cd_env_change(data, var, res);
	var_len = ft_strlen(var);
	node = fetch_envp_node(data->envp, var);
	str = malloc(sizeof(char) * (var_len + ft_strlen(res) + 2));
	if (!str)
		return (ft_perror_message());
	ft_strlcpy(str, var, var_len + 1);
	str[var_len] = '=';
	ft_strlcpy(&str[var_len + 1], res, ft_strlen(res) + 1);
	if (!node)
	{
		if (append_envp_node(&data->envp, str) == -1)
		{
			free(str);
			return (-1);
		}
		return (0);
	}
	free(node->value);
	node->value = str;
	return (0);
}

/*
The function ft_cd_env_change removes the environment variable named var 
from the environment variable list if its new value (res) is empty.
It first checks if the variable already exists and then, 
depending on its position in the list, 
removes it either as the head of the list or from another position.
It is used in the Minishell project to remove outdated or unnecessary environment variables, 
particularly during the update of the working directory, 
where it may be necessary to remove previous values of variables like OLDPWD.
*/
static int	ft_cd_env_change(t_data *data, char *var, char *res)
{
	t_envp	*node;

	if (res)
		return (0);
	node = fetch_envp_node(data->envp, var);
	if (!node)
		return (0);
	if (data->envp == node)
		ft_remove_head_node(&data->envp);
	else
		remove_envp_node(fetch_node_before(&data->envp, var));
	return (0);
}
