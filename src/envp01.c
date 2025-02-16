/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:38:07 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:37:46 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The function `remove_envp_node` removes a node from the environment variable list `envp` that is located after the given node `prev_node`.  
It updates the `next` pointer in `prev_node` to skip the node being removed, and then frees the memory occupied by both its value and the node itself.  
This function prevents memory leaks and ensures the continuity of the list.
*/
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

/*
The function `fetch_node_before` searches for the node that precedes the node in the `envp` 
list containing an environment variable with a name matching the given `key`.  
It calculates the length of the key and then iterates through the list, comparing `key` with the variable names in the nodes.  
If the variable is in the first node, it returns its pointer.  
If the variable is further in the list, it returns the pointer to the node directly preceding it.  
If the variable is not found, it returns `NULL`.  
This function is useful for modifying the list by enabling the removal or updating of the found node.
*/
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

/*
The function `convert_envp_llist_to_array` converts a linked list of environment variables into an array of strings.  
It iterates through the list, copying the values of each variable into a new array, 
and appends a `NULL` pointer at the end.  
This function is used in the *Minishell* project to adapt the format of environment variables to meet the requirements 
of system functions that expect an array as an argument,  
such as when executing external commands.
*/
char	**convert_envp_list_to_array(t_envp *head)
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
