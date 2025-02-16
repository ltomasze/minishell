/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:10:18 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:13:43 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_change_env(char *var, int j, t_data *data);

/*
The export_bltin function handles the export built-in command in the shell. 
It checks whether variables to export have been provided 
and if they have a correct format (starting with a letter or underscore, 
with the remaining characters before = being alphanumeric or underscores). 
If a variable is incorrect, it displays an error message and returns 1. 
For valid variables, it calls ft_change_env to add them to the environment. 
The function ends successfully (0) 
or with an error (1) in case of invalid variables or other issues.
*/
int	export_bltin(char **cmd, t_data *data)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (data->cmd->next || !cmd[1])
		return (1);
	while (cmd[i])
	{
		if (!ft_isalpha(cmd[i][j]) && cmd[i][j] != '_')
			return (ft_error_message("Export: not a valid identifier", 1));
		j++;
		while (cmd[i][j] && cmd[i][j] != '=')
		{
			if (!ft_isalnum(cmd[i][j]) && cmd[i][j] != '_')
				return (1);
			j++;
		}
		if (ft_change_env(cmd[i], j, data) == -1)
			return (1);
		i++;
		j = 0;
	}
	return (0);
}

/*
The ft_change_env function is responsible for modifying or adding environment
variables. It checks whether the provided variable contains the = character,
which separates the variable name from its value. Then, using ft_substr, it
extracts the variable's name. If the variable already exists in envp, its
value is replaced with the new one (provided in var). If the variable does
not exist, it is added to the envp list. In case of an error during memory
allocation or list operations, the function returns -1. After processing the
variable, it frees the memory for variable names and assigns the new values.
*/
static int	ft_change_env(char *var, int j, t_data *data)
{
	t_envp	*node;
	char	*var_name;
	char	*dup_var;

	if (var[j] != '=')
		return (0);
	var_name = ft_substr(var, 0, j);
	if (!var_name)
		return (ft_perror_message());
	dup_var = ft_strdup(var);
	if (!dup_var)
		return (ft_perror_message());
	node = fetch_envp_node(data->envp, var_name);
	free(var_name);
	if (!node)
	{
		if (append_envp_node(&data->envp, dup_var) == -1)
			return (-1);
		return (0);
	}
	free(node->value);
	node->value = dup_var;
	return (0);
}

/*
The `ft_print_env_var` function implements the built-in `export` command  
by printing all environment variables stored in the `envp` structure in the  
`declare -x VAR=value` format. If an argument is passed to the command,  
the function exits with the status code `cmd_exit_status`. Otherwise, it iterates  
through all environment variables and prints them. The function exits with a  
status code of `0`. This is part of the *minishell* project, replicating the  
functionality of the `export` command in this shell interpreter.
*/
int	ft_print_env_var(t_data *data)
{
	t_envp	*envp;

	if (data->cmd->cmd[1])
		exit(data->cmd_exit_status);
	envp = data->envp;
	while (envp)
	{
		printf("declare -x %s\n", envp->value);
		envp = envp->next;
	}
	exit (0);
}
