/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp00.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:38:07 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:35:47 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The function `free_envp` frees the memory of all the nodes in the  
`linked list t_envp`.
*/
void	free_envp(t_envp *head)
{
	t_envp	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

/*
The function `fetch_envp_node` searches the `t_envp` list for a node whose key  
matches the provided `key`. The key is the text before the `=` character in the  
node's value. It returns a pointer to the node if found, or NULL if not.
 */
t_envp	*fetch_envp_node(t_envp *head, char *key)
{
	t_envp	*node;
	size_t	key_len;

	key_len = ft_strlen(key);
	node = head;
	while (node != NULL)
	{
		if (ft_strncmp(key, node->value, key_len) == 0
			&& node->value[key_len] == '=')
			return (node);
		node = node->next;
	}
	return (NULL);
}

/*
The function `increment_shlvl` increments the value of the `SHLVL` environment variable.  
`SHLVL` represents the number of times the shell has been started within the same session.
 */
void	increment_shlvl(t_envp *head)
{
	t_envp	*node;
	char	*shlvl;
	int		shlvl_nb;

	node = fetch_envp_node(head, "SHLVL");
	shlvl = ft_strchr(node->value, '=') + 1;
	shlvl_nb = ft_atoi(shlvl);
	shlvl_nb++;
	shlvl = ft_itoa(shlvl_nb);
	if (!shlvl)
		perror("ft_itoa");
	free(node->value);
	node->value = ft_strjoin("SHLVL=", shlvl);
	if (!node->value)
		perror("ft_strjoin");
	free(shlvl);
}

/*
The function `fetch_envp` converts the `envp` array into a linked list of type `t_envp`.  
It returns a pointer to the head of the list.
 */
t_envp	*fetch_envp(char **envp)
{
	t_envp	*envp_node;
	t_envp	*envp_head;
	t_envp	*envp_node_prev;

	envp_node = NULL;
	envp_head = NULL;
	envp_node_prev = NULL;
	while (*envp != NULL)
	{
		envp_node = malloc(sizeof(t_envp));
		envp_node->value = ft_strdup(*envp);
		if (!envp_node || !envp_node->value)
		{
			free_envp(envp_head);
			return (NULL);
		}
		envp_node->next = NULL;
		if (envp_node_prev != NULL)
			envp_node_prev->next = envp_node;
		else
			envp_head = envp_node;
		envp_node_prev = envp_node;
		envp++;
	}
	return (envp_head);
}

/*
The function `append_envp_node` adds a new node to a singly linked list of type `t_envp` 
(likely a list storing environment variables).  
It works as follows: it starts at the head of the list (`*head`), 
traverses it to reach the last node (if the list is not empty).  
Then, it allocates memory for the new node, sets its `next` pointer to `NULL` 
(ending the list), and assigns the value `str` to the `value` field of the new node.  
If the list was empty, the new node becomes the head of the list.  
The function returns `0` on success and `-1` on failure (e.g., 
memory allocation issue), in which case it prints an error message using `perror`.
*/
int	append_envp_node(t_envp **head, char *str)
{
	t_envp	*node;
	t_envp	*new_node;

	node = *head;
	while (*head && node->next != NULL)
		node = node->next;
	new_node = malloc(sizeof(t_envp));
	if (!new_node)
	{
		perror("append_envp_node");
		return (-1);
	}
	new_node->next = NULL;
	new_node->value = str;
	if (node)
		node->next = new_node;
	else
		node = new_node;
	return (0);
}
