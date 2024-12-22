/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:38:07 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/22 16:45:47 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Funkcja `free_envp` zwalnia pamięć wszystkich węzłów listy połączonej `t_envp`.
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
 * Funkcja `fetch_envp_node` przeszukuje listę `t_envp` w poszukiwaniu węzła, którego klucz odpowiada podanemu `key`.
 * Klucz to tekst występujący przed znakiem '=' w wartości węzła.
 * Zwraca wskaźnik na węzeł, jeśli znajdzie, lub NULL, jeśli nie znajdzie.
 */
t_envp	*fetch_envp_node(t_envp *head, char *key)
{
	t_envp*node;
	size_t	key_len;

	key_len = ft_strlen(key);
	node = head;
	while (node != NULL)
	{
		if (ft_strncmp(key, node->value, key_len) == 0 && node->value[key_len] == '=')
			return (node);
		node = node->next;
	}
	return (NULL);
}
/*
 * Funkcja `increment_shlvl` zwiększa wartość zmiennej środowiskowej `SHLVL`.
 * `SHLVL` odpowiada liczbie razy, kiedy uruchomiono powłokę w tej samej sesji.
 */
void increment_shlvl(t_envp *head)
{
	t_envp *node;
	char	*shlvl;
	int	shlvl_nb;

	node = fetch_envp_node(head, "SHLVL");
	shlvl = ft_strchr(node->value, '=') + 1;
	shlvl_nb = ft_atoi(shlvl);
	shlvl_nb++;
	shlvl = ft_itoa(shlvl_nb);
	if (!shlvl)
		perror("ft_itoa");
	free(node->value);
	if (!node->value)
		perror("ft_strjoin");
	free(shlvl);
}
/*
 * Funkcja `fetch_envp` przekształca tablicę `envp` na listę połączoną `t_envp`.
 * Zwraca wskaźnik na początek listy.
 */
t_envp *fetch_envp (char **envp)
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
		if(!envp_node || !envp_node->value)
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