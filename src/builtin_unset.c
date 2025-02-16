/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:15:05 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:15:52 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The `unset_bltin` function removes environment variables specified  
in the `cmd` array from the `envp` list. For each argument, it creates the name  
(part before the `=` sign) using `ft_substr`. Then, it searches for the variable  
in the `envp` list using `fetch_envp_node`. If the variable exists and is at the  
beginning of the list, it is removed using `ft_remove_head_node`. Otherwise,  
the corresponding node is removed by finding its predecessor with `fetch_node_before`.  
After processing each argument, the function frees memory for the name and exits  
successfully (`0`). If an error occurs, it calls `ft_perror_message`.
*/
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

/*
The `ft_remove_head_node` function removes the first node  
of the environment variable list `envp`. It stores a pointer  
to the next node, frees the memory occupied by the value and  
the first node itself, and then sets the list head pointer  
to the next node. This ensures the list is modified without  
losing data except for the removed element.
*/
void	ft_remove_head_node(t_envp **head)
{
	t_envp	*new_head;

	new_head = (*head)->next;
	free((*head)->value);
	free(*head);
	*head = new_head;
}
