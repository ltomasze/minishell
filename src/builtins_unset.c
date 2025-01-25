/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 16:20:40 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/25 17:08:23 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_remove_head_node(t_envp **head)
{
	t_envp	*new_head;

	new_head = (*head)->next;
	free((*head)->value);
	free(*head);
	*head = new_head;
}

int	unset_bltin(char **cmd, t_data *data)
{
	int		i;
	int		j;
	char	*name;
	t_envp	*node;

	i = 1;
	j = 0;
	while (cmd[i])
	{
		while (cmd[i][j] && cmd[i][j] != '=')
			j++;
		name = ft_substr(cmd[i], 0, j);
		if (!name)
			return (ft_perror_message());
		node = fetch_envp_node(data->envp, name);
		if (!ft_strncmp(name, data->envp->value, j)
			&& data->envp->value[j] == '=')
			ft_remove_head_node(&(data->envp));
		else if (node)
			remove_envp_node(fetch_node_before(&data->envp, name));
		i++;
		j = 0;
		free(name);
	}
	return (0);
}
/*do usuwania zmiennych środowiskowych*/
/*if (!ft_strncmp(name, data->envp->value, j)
			&& data->envp->value[j] == '=')
upewniamy się że zmienna do usunięcia to PATH a nie PATH=
lub PATH=/usr/bin czyli cała zmienna środowiskowa
			ft_remove_head_node(&(data->envp));
jeśli zmienna do usunięcia  znajduje się na początku listy
to używamy tej funkcji*/
/*remove_envp_node(fetch_node_before(&data->envp, name))
funkcja do usuwania zmiennej ze środka listy lub końca*/