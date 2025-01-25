/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 14:00:43 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/25 14:05:06 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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