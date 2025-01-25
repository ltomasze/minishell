/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 16:22:30 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/25 18:50:57 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**convert_envp_llist_to_array(t_envp *head)
{
	t_envp	*node;
	size_t	nodes_cnt;
	size_t	i;
	char	**arr;

	nodes_cnt = 0;
	node = head;
	while (node && ++nodes_cnt)
		node = node->next;
	arr = malloc(sizeof(char *) * (nodes_cnt + 1));
	if (!arr)
		return (NULL);
	node = head;
	i = 0;
	while (node)
	{
		arr[i] = ft_strdup(node->value);
		if (!arr[i])
			return (NULL);
		i++;
		node = node->next;
	}
	arr[i] = NULL;
	return (arr);
}

t_envp	*fetch_node_before(t_envp **head, char *key)
{
	t_envp	*p_node;
	int		key_len;

	key_len = ft_strlen(key);
	if (!(ft_strncmp(key, (*head)->value, key_len))
		&& (*head)->value[key_len] == '=')
		return (*head);
	p_node = *head;
	while (p_node)
	{
		if (!p_node->next)
			return (NULL);
		else if (!p_node->next->value)
			p_node = p_node->next;
		else if (!(ft_strncmp(key, p_node->next->value, key_len))
			&& p_node->next->value[key_len] == '=')
			return (p_node);
		else
			p_node = p_node->next;
	}
	return (NULL);
}

void	remove_envp_node(t_envp *prev_node)
{
	t_envp	*node;

	node = prev_node->next;
	if (node->next != NULL)
		prev_node->next = node->next;
	else
		prev_node->next = NULL;
	free (node->value);
	free (node);
}